#include "TES3AnimationGroup.h"

#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Sound.h"

#include "NIKeyframeManager.h"

#include "Log.h"

namespace TES3 {
	static constexpr auto DEBUG_ANIM_PARSER = false;

	/// <summary>
	/// AnimationGroup
	/// </summary>

	const auto TES3_AnimationGroup_ctor = reinterpret_cast<AnimationGroup * (__thiscall*)(AnimationGroup*, int)>(0x4927F0);
	AnimationGroup* AnimationGroup::ctor(int animGroupId) {
		TES3_AnimationGroup_ctor(this, animGroupId);

		// Extra setup for the new anim parser.
		patchedRootTravelSpeed = 0;
		if (actionCount > 0) {
			actionTimings = mwse::tes3::_new<float>(actionCount);
			memset(actionTimings, 0, sizeof(float) * actionCount);
		}

		return this;
	}

	const auto TES3_AnimationGroup_dtor = reinterpret_cast<void(__thiscall*)(AnimationGroup*)>(0x492880);
	void AnimationGroup::dtor() {
		// Clean up lua events.
		for (auto& key : getSoundGenKeys()) {
			if (key.hasLuaEvent()) {
				key.freeLuaEvent();
			}
		}

		TES3_AnimationGroup_dtor(this);
	}

	const auto TES3_AnimationGroup_calcNoteTimes = reinterpret_cast<void(__thiscall*)(AnimationGroup*)>(0x492B70);
	void AnimationGroup::calcNoteTimes() {
		TES3_AnimationGroup_calcNoteTimes(this);
	}

	const auto TES3_AnimationGroup_setSoundGenCount = reinterpret_cast<void(__thiscall*)(AnimationGroup*, unsigned int)>(0x492980);
	void AnimationGroup::setSoundGenCount(unsigned int newCount) {
		TES3_AnimationGroup_setSoundGenCount(this, newCount);
	}

	const auto TES3_AnimationGroup_setSoundGenVolume = reinterpret_cast<void(__thiscall*)(AnimationGroup*, unsigned int, float)>(0x492B20);
	void AnimationGroup::setSoundGenVolume(unsigned int index, float volume) {
		TES3_AnimationGroup_setSoundGenVolume(this, index, volume);
	}

	const auto TES3_AnimationGroup_setSoundGenPitch = reinterpret_cast<void(__thiscall*)(AnimationGroup*, unsigned int, float)>(0x492B50);
	void AnimationGroup::setSoundGenPitch(unsigned int index, float pitch) {
		TES3_AnimationGroup_setSoundGenPitch(this, index, pitch);
	}

	AnimationGroup* AnimationGroup::findGroup(AnimGroupID id) {
		for (auto iter = this; iter; iter = iter->nextGroup) {
			if (iter->groupId == id) {
				return iter;
			}
		}
		return nullptr;
	}

	const auto TES3_data_animGroupActionClasses = reinterpret_cast<int*>(0x78B0A8);
	AnimGroupActionClass AnimationGroup::getActionClass(AnimGroupID id) {
		int actionClass = TES3_data_animGroupActionClasses[int(id)];
		return AnimGroupActionClass(actionClass);
	}

	nonstd::span<int> AnimationGroup::getActionFrames() {
		return { actionFrames, actionCount };
	}

	nonstd::span<float> AnimationGroup::getActionTimings() {
		return { actionTimings, actionCount };
	}

	nonstd::span<AnimationGroup::SoundGenKey> AnimationGroup::getSoundGenKeys() {
		return { soundGenKeys, soundGenCount };
	}

	AnimationGroup::LuaEvent::LuaEvent(const std::string_view& _id, const std::string_view& _param) :
		tag(eventTag),
		refCount(0),
		id(_id),
		param(_param)
	{

	}

	AnimationGroup::LuaEvent::~LuaEvent() {
		if constexpr (DEBUG_ANIM_PARSER) {
			mwse::log::getLog() << fmt::format("[AnimParser] Cleaning up lua event. ID: '{}'; Param: '{}'\n", id, param);
		}
	}

	std::string AnimationGroup::LuaEvent::toString() const {
		return fmt::format("{} {}", id, param);
	}

	AnimationGroup::LuaEvent* AnimationGroup::LuaEvent::toEvent(Sound* sound) {
		LuaEvent* e = reinterpret_cast<LuaEvent*>(sound);
		return (e && e->tag == eventTag) ? e : nullptr;
	}

	bool AnimationGroup::SoundGenKey::hasLuaEvent() const {
		return LuaEvent::toEvent(sound) != nullptr;
	}

	void AnimationGroup::SoundGenKey::freeLuaEvent() {
		if (!event) {
			return;
		}

		event->refCount--;
		if (event->refCount == 0) {
			delete event;
		}
		event = nullptr;
	}

	Sound* AnimationGroup::SoundGenKey::getSound() const {
		if (hasLuaEvent()) {
			return nullptr;
		}
		return sound;
	}

	void AnimationGroup::SoundGenKey::setSound(Sound* sound) {
		this->sound = sound;
	}

	AnimationGroup::LuaEvent* AnimationGroup::SoundGenKey::getLuaEvent() const {
		if (!hasLuaEvent()) {
			return nullptr;
		}
		return event;
	}

	void AnimationGroup::SoundGenKey::setLuaEvent(LuaEvent* e) {
		if (event && event != e) {
			freeLuaEvent();
		}
		event = e;
		if (event) {
			event->refCount++;
		}
	}

	/// <summary>
	/// KeyframeDefinition
	/// </summary>

	const auto TES3_KeyframeDefinitionVanilla_ctor = reinterpret_cast<KeyframeDefinitionVanilla * (__thiscall*)(KeyframeDefinitionVanilla*, const char*, const char*)>(0x4EDBF0);
	KeyframeDefinition* KeyframeDefinition::ctor(const char* nifPath, const char* name) {
		// Construct new members first, because they are used during parsing in the base constructor.
		::new (&namedGroups) decltype(namedGroups);

		// Call base class ctor.
		TES3_KeyframeDefinitionVanilla_ctor(this, nifPath, name);
		return this;
	}

	const auto TES3_KeyframeDefinitionVanilla_dtor = reinterpret_cast<void (__thiscall*)(KeyframeDefinitionVanilla*)>(0x4EDCD0);
	void KeyframeDefinition::dtor() {
		// Clean up new members.
		namedGroups.~decltype(namedGroups)();

		// Call base class dtor.
		TES3_KeyframeDefinitionVanilla_dtor(this);
	}

	/// <summary>
	/// Animation text key parser.
	/// </summary>

	// Replicate Morrowind's 15 fps action timing. Used for testing only.
	int timeToFrameNumber(float time) {
		return std::lrint(15.0f * time);
	}

	float frameNumberToTime(int frame) {
		return frame / 15.0f;
	}

	// Case-insensitive operations for string_view.
	bool textCIEquals(const std::string_view& text, const std::string_view& s) {
		if (text.length() != s.length()) {
			return false;
		}
		return _strnicmp(text.data(), s.data(), text.length()) == 0;
	}

	bool textCIEquals(const std::string_view& text, const char* s) {
		if (text.length() != std::strlen(s)) {
			return false;
		}
		return _strnicmp(text.data(), s, text.length()) == 0;
	}

	bool textCIStartsWith(const std::string_view& text, const char* startsWith) {
		return _strnicmp(text.data(), startsWith, std::strlen(startsWith)) == 0;
	}

	std::string textAsLowercase(const std::string_view& text) {
		std::string lowered{ text };
		std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) { return std::tolower(c); });
		return lowered;
	}

	struct TextKeyParser {
		using string = std::string;
		using string_view = std::string_view;

		struct ActionIndex {
			enum {
				Looping_Start = 0,
				Looping_End = 1,
				CreatureAttack_End = 2,
				Looping_LoopStart = 2,
				Looping_LoopEnd = 3,
			};
		};

		KeyframeDefinition* kfData;
		AnimationGroup* firstGroup;
		AnimationGroup* lastGroup;
		std::vector<AnimationGroup*> activeAnimGroups;
		std::vector<AnimationGroup*> expireGroups;
		Creature* matchedCreature;

		static inline std::unordered_map<string, AnimGroupID> mapAnimationNames;

		TextKeyParser(KeyframeDefinition* _kfData) : kfData(_kfData), firstGroup(nullptr), lastGroup(nullptr), matchedCreature(nullptr) {}

		int parse(NI::Sequence* sequence, const char* meshPath);
		void parseNoteAction(const NI::TextKey& key, std::string_view noteKey, std::string_view noteValue);
		void parseNoteSound(const NI::TextKey& key, std::string_view noteKey, std::string_view noteValue);
		void parseNoteLuaEvent(const NI::TextKey& key, std::string_view noteKey, std::string_view noteValue);
		float measureRootMovementSpeedOverLoop(NI::TimeController* rootController, AnimationGroup* animGroup);
		void testResult(NI::Sequence* sequence, const char* meshPath);
		static void preCacheMappings();
	};

	const auto TES3_soundGenGenericNames = reinterpret_cast<const char**>(0x7B064C);
	const auto TES3_soundGenGenericNamesEnd = TES3_soundGenGenericNames + 8;
	const auto TES3_animGroupNames = reinterpret_cast<const char**>(0x78A970);
	const auto TES3_animGroupActionIds = reinterpret_cast<int*>(0x78B300);
	const auto TES3_animActionTextByActionClass = reinterpret_cast<const char**>(0x78ABC8);

	int TextKeyParser::parse(NI::Sequence* sequence, const char* meshPath) {
		// Pre-convert animation string tables.
		preCacheMappings();

		// Find creature for soundgens by matching meshPath.
		auto records = DataHandler::get()->nonDynamicData;
		matchedCreature = nullptr;
		for (auto iterObject : *records->list) {
			if (iterObject->objectType == ObjectType::Creature) {
				auto creatureMeshPath = iterObject->getModelPath();
				if (_stricmp(creatureMeshPath, meshPath) == 0) {
					matchedCreature = static_cast<Creature*>(iterObject);
					break;
				}
			}
		}

		// Currently active animations.
		// Animation notes can overlap multiple anims, but only for action classes 0-2.
		AnimationGroup* lastAnimGroup = nullptr;
		if constexpr (DEBUG_ANIM_PARSER) {
			mwse::log::getLog() << "[AnimParser] Parsing kf text keys for " << meshPath << std::endl;
		}

		for (auto& key : sequence->textKeys->getKeys()) {
			if (!key.text) {
				continue;
			}

			// Process multi-line text key.
			const char* p = key.text;

			while (*p != '\0') {
				// Trim newlines.
				while (*p == '\r' || *p == '\n') { ++p; }

				// Search for end of note. Trim trailing spaces.
				const char* noteBegin = p;
				while (*p != '\0' && *p != '\r' && *p != '\n') { ++p; }
				const char* noteEnd = p;
				while (noteEnd != noteBegin && noteEnd[-1] == ' ') { --noteEnd; }
				string_view note{ noteBegin, (size_t)std::distance(noteBegin, noteEnd) };

				// Split note into key and value.
				auto splitAt = std::find(note.begin(), note.end(), ':');
				if (splitAt != note.end()) {
					auto valueBegin = splitAt + 1;
					while (valueBegin != note.end() && *valueBegin == ' ') { ++valueBegin; }

					string_view noteKey{ noteBegin, (size_t)std::distance(note.begin(), splitAt) };
					string_view noteValue{ &*valueBegin, (size_t)std::distance(valueBegin, note.end()) };

					// Dispatch based on key name.
					if (textCIEquals(noteKey, "Sound") || textCIEquals(noteKey, "SoundGen")) {
						parseNoteSound(key, noteKey, noteValue);
					}
					else if (textCIEquals(noteKey, "LuaEvent")) {
						parseNoteLuaEvent(key, noteKey, noteValue);
					}
					else {
						parseNoteAction(key, noteKey, noteValue);
					}
				}

				// Skip to next newline.
				while (*p != '\0' && *p != '\n') { ++p; }
			}

			// Remove expiring groups after all other notes in the key have been applied.
			auto iterErase = std::remove_if(
				activeAnimGroups.begin(), activeAnimGroups.end(),
				[&](const AnimationGroup* g) {
					return std::find(expireGroups.begin(), expireGroups.end(), g) != expireGroups.end();
				}
			);
			activeAnimGroups.erase(iterErase, activeAnimGroups.end());
			expireGroups.clear();
		}

		// Calculate root movement and count groups.
		auto rootController = sequence->getController("Bip01");
		if (rootController == nullptr) {
			rootController = sequence->getController("Root Bone");
		}

		int groupCount = kfData->namedGroups.size();
		for (auto g = firstGroup; g; g = g->nextGroup) {
			++groupCount;

			if (rootController != nullptr) {
				auto actionClass = AnimationGroup::getActionClass(g->groupId);
				if (actionClass == AnimGroupActionClass::Looping) {
					// Root movement measurement is over a loop cycle.
					auto travelSpeed = measureRootMovementSpeedOverLoop(rootController, g);
					g->patchedRootTravelSpeed = short(std::lrint(travelSpeed));
				}
			}
		}

		// Write to output pointer before finishing.
		kfData->animationGroups = firstGroup;
		return groupCount;
	}

	void TextKeyParser::parseNoteAction(const NI::TextKey& key, std::string_view noteKey, std::string_view noteValue) {
		// Check for animation name. Vanilla animations are indexed by an id, while all others are by string.
		string lowercaseName = textAsLowercase(noteKey);
		bool isNamedAnim = false;
		AnimGroupID matchedGroupId = AnimGroupID::Idle9;
		AnimationGroup* newGroup = nullptr;

		auto iterAnimName = mapAnimationNames.find(lowercaseName);
		if (iterAnimName != mapAnimationNames.end()) {
			// Vanilla name.
			matchedGroupId = iterAnimName->second;
		}
		else {
			// Non-vanilla, treat as named animation.
			isNamedAnim = true;
			auto itt = kfData->namedGroups.find(lowercaseName);
			if constexpr (DEBUG_ANIM_PARSER) {
				mwse::log::getLog() << "[AnimParser] Named animation: '" << noteKey << "'" << std::endl;
			}

			// A behave-as-group-id is required to get the correct action class.
			if (itt != kfData->namedGroups.end()) {
				matchedGroupId = itt->second->groupId;
			}
			else if (textCIStartsWith(noteValue, "AsGroup ")) {
				// Check for behave-as-group assignment key. Named anims without AsGroup default to Idle9, set above.
				string asGroup = textAsLowercase(noteValue.substr(8));
				iterAnimName = mapAnimationNames.find(asGroup);
				matchedGroupId = (iterAnimName != mapAnimationNames.end()) ? iterAnimName->second : AnimGroupID::Idle9;
				if constexpr (DEBUG_ANIM_PARSER) {
					mwse::log::getLog() << "[AnimParser] '" << lowercaseName << "' AsGroup '" << asGroup << "' = " << int(matchedGroupId) << std::endl;
				}
				{
					// Clear all active anims.
					activeAnimGroups.clear();

					// Construct new anim group.
					newGroup = mwse::tes3::_new<AnimationGroup>();
					newGroup->ctor(int(matchedGroupId));
					activeAnimGroups.emplace_back(newGroup);
				}
			}
		}

		auto actionClass = AnimationGroup::getActionClass(matchedGroupId);
		auto iterActionIndex = &TES3_animGroupActionIds[39 * int(matchedGroupId)];

		for (int i = 0; i < 39 && *iterActionIndex != -1; ++i, ++iterActionIndex) {
			auto actionIndex = *iterActionIndex;
			auto actionText = TES3_animActionTextByActionClass[8 * actionIndex + int(actionClass)];

			// Note this is a prefix match. e.g. actionText could be "Stop."
			if (textCIStartsWith(noteValue, actionText)) {
				// Find if the animation group exists already. Check active anim groups first, then all groups.
				// Activate group if not already active.
				AnimationGroup* animGroup = nullptr;
				auto iterMatchedGroup = std::find_if(
					activeAnimGroups.begin(), activeAnimGroups.end(),
					[&](AnimationGroup* g) { return g->groupId == matchedGroupId; }
				);
				if (iterMatchedGroup != activeAnimGroups.end()) {
					animGroup = *iterMatchedGroup;
				}
				else if (isNamedAnim) {
					auto itt = kfData->namedGroups.find(lowercaseName);
					if (itt != kfData->namedGroups.end()) {
						animGroup = itt->second;
						activeAnimGroups.emplace_back(animGroup);
					}
				}
				else {
					for (auto g = firstGroup; g; g = g->nextGroup) {
						if (g->groupId == matchedGroupId) {
							animGroup = g;
							activeAnimGroups.emplace_back(animGroup);
							break;
						}
					}
				}

				if (animGroup == nullptr) {
					// Sound notes can apply to multiple overlapping anims, but only for action classes 0-2.
					// On new anim group, clear all active non-overlapping action class anims.
					auto iterErase = std::remove_if(
						activeAnimGroups.begin(), activeAnimGroups.end(),
						[&](const AnimationGroup* g) {
							return AnimationGroup::getActionClass(g->groupId) > AnimGroupActionClass::CreatureAttack;
						}
					);
					activeAnimGroups.erase(iterErase, activeAnimGroups.end());

					// Construct new anim group.
					newGroup = mwse::tes3::_new<AnimationGroup>();
					newGroup->ctor(int(matchedGroupId));
					activeAnimGroups.emplace_back(newGroup);
					animGroup = newGroup;
				}

				// Set action timing. The conditional is a safety check to avoid array overrun.
				if (actionIndex < animGroup->actionCount) {
					animGroup->actionTimings[actionIndex] = key.time;
					animGroup->actionFrames[actionIndex] = timeToFrameNumber(key.time);

					// Propagate Start/End timing to missing Loop Start/Loop End timing.
					if (actionClass == AnimGroupActionClass::Looping) {
						if (actionIndex == 0 && animGroup->actionFrames[ActionIndex::Looping_LoopStart] == 0) {
							animGroup->actionTimings[ActionIndex::Looping_LoopStart] = animGroup->actionTimings[actionIndex];
							animGroup->actionFrames[ActionIndex::Looping_LoopStart] = animGroup->actionFrames[actionIndex];
						}
						else if (actionIndex == 1 && animGroup->actionFrames[ActionIndex::Looping_LoopEnd] == 0) {
							animGroup->actionTimings[ActionIndex::Looping_LoopEnd] = animGroup->actionTimings[actionIndex];
							animGroup->actionFrames[ActionIndex::Looping_LoopEnd] = animGroup->actionFrames[actionIndex];
						}
					}
				}

				// Sound notes can apply to multiple overlapping anims, but only for action classes 0-2.
				// Remove them from the active list on their end actions.
				switch (actionClass) {
				case AnimGroupActionClass::NonLooping:
				case AnimGroupActionClass::Looping:
					if (actionIndex == ActionIndex::Looping_End) {
						expireGroups.emplace_back(animGroup);
					}
					break;
				case AnimGroupActionClass::CreatureAttack:
					if (actionIndex == ActionIndex::CreatureAttack_End) {
						expireGroups.emplace_back(animGroup);
					}
					break;
				}
				break;
			}
		}

		if (newGroup) {
			if (isNamedAnim) {
				kfData->namedGroups[lowercaseName] = newGroup;
			}
			else {
				// Append to anim group chain.
				if (lastGroup) {
					lastGroup->nextGroup = newGroup;
				}
				else {
					firstGroup = newGroup;
				}
				lastGroup = newGroup;
			}
		}
	}

	void TextKeyParser::parseNoteSound(const NI::TextKey& key, std::string_view noteKey, std::string_view noteValue) {
		auto records = DataHandler::get()->nonDynamicData;
		Sound* matchedSound = nullptr;
		std::optional<float> volumeParam, pitchParam;

		// Sounds and soundgens can have volume and pitch parameters.
		// Sound ids can have spaces in them, but spaces are also parameter separators.
		// The parser first checks for ',' and if it exists accepts spaces in the value, else falls back to ' '.
		auto param1 = std::find(noteValue.begin(), noteValue.end(), ',');
		if (param1 == noteValue.end()) {
			param1 = std::find(noteValue.begin(), noteValue.end(), ' ');
		}
		if (param1 != noteValue.end()) {
			while (*param1 == ' ') { ++param1; }
			volumeParam = float(std::atof(&*param1));
		}
		auto param2 = std::find(param1, noteValue.end(), ',');
		if (param2 == noteValue.end()) {
			param2 = std::find(param1, noteValue.end(), ' ');
		}
		if (param2 != noteValue.end()) {
			while (*param2 == ' ') { ++param2; }
			pitchParam = float(std::atof(&*param2));
		}

		if (textCIEquals(noteKey, "SoundGen")) {
			// Convert soundgens to sounds based on creature.
			auto iterSoundGenName = std::find_if(
				TES3_soundGenGenericNames, TES3_soundGenGenericNamesEnd,
				[&](const char* x) { return textCIStartsWith(noteValue, x); }
			);
			if (iterSoundGenName != TES3_soundGenGenericNamesEnd) {
				int index = iterSoundGenName - TES3_soundGenGenericNames;
				matchedSound = records->getSoundGeneratorSound(matchedCreature, index);
			}
		}
		else { // noteKey is "Sound"
			string id{ noteValue };
			matchedSound = records->findSound(id.c_str());
		}

		if (matchedSound) {
			// Apply sound to active groups.
			for (auto animGroup : activeAnimGroups) {
				int newIndex = animGroup->soundGenCount;
				animGroup->setSoundGenCount(animGroup->soundGenCount + 1);
				auto soundGen = &animGroup->soundGenKeys[newIndex];

				soundGen->startTime = key.time;
				soundGen->startFrame = timeToFrameNumber(key.time);
				soundGen->sound = matchedSound;
				if (volumeParam) {
					animGroup->setSoundGenVolume(newIndex, volumeParam.value());
				}
				if (pitchParam) {
					animGroup->setSoundGenPitch(newIndex, pitchParam.value());
				}
			}
		}
	}

	void TextKeyParser::parseNoteLuaEvent(const NI::TextKey& key, std::string_view noteKey, std::string_view noteValue) {
		string_view eventParam;

		// Decode event name and parameter. The parameter is the first text after a space.
		auto iterSep = std::find(noteValue.begin(), noteValue.end(), ' ');
		string_view eventName{ &*noteValue.begin(), (size_t)std::distance(noteValue.begin(), iterSep) };

		if (iterSep != noteValue.end()) {
			auto iterParam = iterSep;
			while (*iterParam == ' ') { ++iterParam; }
			eventParam = { &*iterParam, (size_t)std::distance(iterParam, noteValue.end()) };
		}

		if (eventName.empty()) {
			return;
		}

		// Create new event data and place it in the soundgen array of active groups.
		if constexpr (DEBUG_ANIM_PARSER) {
			mwse::log::getLog() << "[AnimParser] LuaEvent name=" << eventName << " param=" << eventParam << std::endl;
		}
		const auto newEvent = new AnimationGroup::LuaEvent(eventName, eventParam);
		for (auto animGroup : activeAnimGroups) {
			int newIndex = animGroup->soundGenCount;
			animGroup->setSoundGenCount(animGroup->soundGenCount + 1);
			auto soundGen = &animGroup->soundGenKeys[newIndex];

			soundGen->startTime = key.time;
			soundGen->startFrame = timeToFrameNumber(key.time);
			soundGen->setLuaEvent(newEvent);
		}
	}

	float TextKeyParser::measureRootMovementSpeedOverLoop(NI::TimeController* rootController, AnimationGroup* animGroup) {
		float loopStartTime = animGroup->actionTimings[ActionIndex::Looping_LoopStart];
		float loopEndTime = animGroup->actionTimings[ActionIndex::Looping_LoopEnd];

		if (loopEndTime > loopStartTime) {
			// Invoke controller on temp node instead of activating the sequence.
			NI::Node root;
			NI::ObjectNET* target = &root;

			std::swap(rootController->target, target);
			rootController->setActive(true);
			rootController->vTable.asController->update(rootController, loopStartTime);
			Vector3 startPoint = root.localTranslate;
			rootController->vTable.asController->update(rootController, loopEndTime);
			Vector3 movement = root.localTranslate - startPoint;
			rootController->setActive(false);

			// Note that movement is measured in the XY plane.
			std::swap(rootController->target, target);
			movement.z = 0;
			return movement.length() / (loopEndTime - loopStartTime);
		}
		return 0.0f;
	}

	const auto TES3_parseSeqTextKeysToAnimGroups = reinterpret_cast<int(__cdecl*)(NI::Sequence*, const char*, AnimationGroup**)>(0x4C30F0);
	void TextKeyParser::testResult(NI::Sequence* sequence, const char* meshPath) {
		// Compare to vanilla. Note that the vanilla parser will modify the text keys in-place.
		AnimationGroup* standardParseGroups = nullptr;
		int standardParseCount = TES3_parseSeqTextKeysToAnimGroups(sequence, meshPath, &standardParseGroups);

		// Compare total anim groups found.
		int newParseCount = 0;
		for (const AnimationGroup* agNew = kfData->animationGroups; agNew; agNew = agNew->nextGroup, ++newParseCount) {}
		mwse::log::getLog() << "[AnimParser] Testing anim=" << meshPath << std::endl;
		mwse::log::getLog() << "[AnimParser] Group count std=" << standardParseCount << " new=" << newParseCount << std::endl;

		const AnimationGroup* agStd = standardParseGroups;
		for (const AnimationGroup* agNew = kfData->animationGroups; agNew && agStd; agNew = agNew->nextGroup, agStd = agStd->nextGroup) {
			mwse::log::getLog() << "[AnimParser] Checking group=" << TES3_animGroupNames[int(agNew->groupId)] << std::endl;

			// Compare group id.
			if (agNew->groupId != agStd->groupId) {
				mwse::log::getLog() << "[AnimParser] TEST FAIL groupId std=" << TES3_animGroupNames[int(agStd->groupId)] << " new=" << TES3_animGroupNames[int(agNew->groupId)] << std::endl;
			}

			// Compare if actions match, using frame numbers. Action timing is a deferred calculation in vanilla.
			if (agNew->actionCount != agStd->actionCount) {
				mwse::log::getLog() << "[AnimParser] TEST FAIL actionCount std=" << int(agStd->actionCount) << " new=" << int(agNew->actionCount) << std::endl;
			}
			else {
				if (memcmp(agNew->actionFrames, agStd->actionFrames, agStd->actionCount * sizeof(agStd->actionFrames[0])) != 0) {
					mwse::log::getLog() << "[AnimParser] TEST FAIL actionFrames" << std::endl;
					for (unsigned int i = 0; i < agStd->actionCount; ++i) {
						mwse::log::getLog() << "[AnimParser]     act " << i << ": std=" << agStd->actionFrames[i] << " new=" << agNew->actionFrames[i] << std::endl;
					}
				}
			}

			// Compare sounds. Sound timing is a deferred calculation in vanilla.
			// Vanilla can have sound events past the end of the animation, so these are excluded from tests.
			unsigned int i, stdSoundGenCount = 0, newSoundGenCount = 0;
			if (agStd->actionCount > 0) {
				auto lastActionFrame = agStd->actionFrames[agStd->actionCount - 1];

				for (i = 0; i < agStd->soundGenCount; ++i) {
					if (agStd->soundGenKeys[i].startFrame <= lastActionFrame) {
						stdSoundGenCount++;
					}
				}
				for (i = 0; i < agNew->soundGenCount; ++i) {
					if (agNew->soundGenKeys[i].startFrame <= lastActionFrame && !AnimationGroup::LuaEvent::toEvent(agNew->soundGenKeys[i].sound)) {
						newSoundGenCount++;
					}
				}
			}
			if (newSoundGenCount != stdSoundGenCount) {
				mwse::log::getLog() << "[AnimParser] TEST FAIL soundGenCount std=" << int(stdSoundGenCount) << " new=" << int(newSoundGenCount) << std::endl;
			}
			else {
				for (unsigned int i = 0; i < stdSoundGenCount; ++i) {
					auto s1 = &agStd->soundGenKeys[i], s2 = &agNew->soundGenKeys[i];
					// startTime is a deferred calculation in vanilla.
					if (s1->startFrame != s2->startFrame
						|| s1->volume != s2->volume
						|| s1->pitch != s2->pitch
						|| s1->sound != s2->sound) {
						mwse::log::getLog() << "[AnimParser] TEST FAIL soundGenKeys" << std::endl;
						mwse::log::getLog() << "    sound std " << i << ": frame=" << s1->startFrame << ", timing=" << s1->startTime << ", vol=" << int(s1->volume) << ", pitch=" << s1->pitch << ", sound=" << std::hex << (s1->sound ? s1->sound->id : "(null)") << std::dec << std::endl;
						mwse::log::getLog() << "    sound new " << i << ": frame=" << s2->startFrame << ", timing=" << s2->startTime << ", vol=" << int(s2->volume) << ", pitch=" << s2->pitch << ", sound=" << std::hex << (s2->sound ? s2->sound->id : "(null)") << std::dec << std::endl;
					}
				}
			}
		}

		mwse::log::getLog() << "[AnimParser] Testing end anim=" << meshPath << std::endl;
	}

	void TextKeyParser::preCacheMappings() {
		if (mapAnimationNames.empty()) {
			// Build animation name to id map.
			for (auto i = int(AnimGroupID::First); i <= int(AnimGroupID::Last); ++i) {
				string lowercaseName{ TES3_animGroupNames[i] };
				std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), [](unsigned char c) { return std::tolower(c); });
				mapAnimationNames.emplace(lowercaseName, AnimGroupID(i));
			}
		}
	}

	std::string KeyframeDefinition::toCanonicalName(std::string_view name) {
		return textAsLowercase(name);
	}

	int __cdecl KeyframeDefinition::parseSeqTextKeysToAnimGroups(NI::Sequence* sequence, const char* meshPath, KeyframeDefinition* kfData) {
		kfData->animationGroups = nullptr;
		if (!sequence || !sequence->textKeys) {
			return 0;
		}

		// Call replacement parser.
		TextKeyParser parser(kfData);
		auto groupCount = parser.parse(sequence, meshPath);

		// Test against vanilla function.
		if constexpr (DEBUG_ANIM_PARSER) {
			parser.testResult(sequence, meshPath);
		}

		return groupCount;
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::AnimationGroup)
