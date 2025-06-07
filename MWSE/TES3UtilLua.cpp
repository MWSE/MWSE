#include "TES3UtilLua.h"

#include "LuaManager.h"

#include "TES3GameFile.h"

#include "TES3Util.h"
#include "UIUtil.h"
#include "LuaUtil.h"
#include "Stack.h"
#include "Log.h"
#include "ScriptUtil.h"
#include "CodePatchUtil.h"
#include "LuaObject.h"
#include "MemoryUtil.h"
#include "MWSEConfig.h"

#include "NICamera.h"
#include "NINode.h"
#include "NIPick.h"
#include "NIRTTI.h"
#include "NIStream.h"
#include "NITriShape.h"

#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3AIData.h"
#include "TES3AIPackage.h"
#include "TES3Alchemy.h"
#include "TES3AnimationData.h"
#include "TES3AnimationGroup.h"
#include "TES3Archive.h"
#include "TES3Armor.h"
#include "TES3AudioController.h"
#include "TES3Birthsign.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Container.h"
#include "TES3CombatSession.h"
#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Faction.h"
#include "TES3Fader.h"
#include "TES3Game.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicInstanceController.h"
#include "TES3Misc.h"
#include "TES3MobManager.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3PlayerAnimationController.h"
#include "TES3Quest.h"
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3Region.h"
#include "TES3Script.h"
#include "TES3ScriptCompiler.h"
#include "TES3Sound.h"
#include "TES3SoundGenerator.h"
#include "TES3Spell.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

#include "LuaCalcBarterPriceEvent.h"
#include "LuaCalcRepairPriceEvent.h"
#include "LuaCalcSpellPriceEvent.h"
#include "LuaCalcTrainingPriceEvent.h"
#include "LuaCalcTravelPriceEvent.h"
#include "LuaEnchantChargeUseEvent.h"
#include "LuaMusicChangeTrackEvent.h"

#include "BitUtil.h"
#include "MathUtil.h"

namespace mwse::lua {
	//
	// Helper functions.
	//

	auto iterateObjectsFiltered(const std::unordered_set<unsigned int> desiredTypes) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			throw std::runtime_error("Function called before game initialization.");
		}
		auto ndd = dataHandler->nonDynamicData;

		// Prepare the lists we care about.
		std::queue<TES3::Object*> objectListQueue;
		auto searchingSpells = desiredTypes.count(TES3::ObjectType::Spell) > 0; // TODO: Change in C++20 to .contains.
		if (searchingSpells) {
			objectListQueue.push(ndd->spellsList->head);
		}
		if (desiredTypes.size() != 1 || !searchingSpells) {
			objectListQueue.push(ndd->list->head);
		}

		// Get the first reference we care about.
		TES3::Object* object = nullptr;
		if (!objectListQueue.empty()) {
			object = objectListQueue.front();
			objectListQueue.pop();
		}

		return [object, objectListQueue, desiredTypes]() mutable -> TES3::BaseObject* {
			while (object && !desiredTypes.empty() && !desiredTypes.count(object->objectType)) {
				object = reinterpret_cast<TES3::Reference*>(object->nextInCollection);

				// If we hit the end of the list, check for the next list.
				if (object == nullptr && !objectListQueue.empty()) {
					object = objectListQueue.front();
					objectListQueue.pop();
				}
			}

			if (object == nullptr) {
				return nullptr;
			}

			// Get the object we want to return.
			auto ret = object;

			// Get the next reference. If we're at the end of the list, go to the next one
			object = reinterpret_cast<TES3::Reference*>(object->nextInCollection);
			if (object == nullptr && !objectListQueue.empty()) {
				object = objectListQueue.front();
				objectListQueue.pop();
			}

			return ret;
		};
	}

	//
	// tes3.* functions.
	//

	TES3::Reference* getPlayerRef() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			auto mobilePlayer = worldController->getMobilePlayer();
			if (mobilePlayer) {
				return mobilePlayer->reference;
			}
		}
		return nullptr;
	}

	TES3::MobilePlayer* getMobilePlayer() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->getMobilePlayer();
		}
		return nullptr;
	}

	TES3::Cell* getPlayerCell() {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->currentCell;
		}
		return nullptr;
	}

	TES3::Reference* getPlayerTarget() {
		auto game = TES3::Game::get();
		if (game) {
			return game->playerTarget;
		}
		return nullptr;
	}

	TES3::Reference* getReference(sol::optional<const char*> id) {
		if (id) {
			return tes3::getReference(id.value());
		}
		else {
			return LuaManager::getInstance().getCurrentReference();
		}
	}

	TES3::BaseObject* getObject(const char* id) {
		if (!id) {
			throw std::invalid_argument("Invalid first parameter provided. Must be a string ID.");
		}

		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->resolveObject(id);
		}
		return nullptr;
	}

	bool deleteObject(sol::object maybe) {
		auto object = maybe.as<TES3::BaseObject*>();
		if (object) {
			TES3::DataHandler::get()->nonDynamicData->deleteObject(object);
			object->vTable.base->deleting_dtor(object, true);
			return true;
		}
		return false;
	}

	TES3::Script* getScript(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->findScriptByName(id);
		}
		return nullptr;
	};

	sol::optional<double> getGlobal(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			TES3::GlobalVariable* global = dataHandler->nonDynamicData->findGlobalVariable(id);
			if (global) {
				return global->getValue_lua();
			}
		}
		return {};
	}

	bool setGlobal(const std::string& id, double value) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			TES3::GlobalVariable* global = dataHandler->nonDynamicData->findGlobalVariable(id.c_str());
			if (global) {
				global->setValue_lua(value);
				return true;
			}
		}
		return false;
	}

	TES3::GlobalVariable* findGlobal(const std::string& id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->findGlobalVariable(id.c_str());
		}
		return nullptr;
	}

	TES3::GameSetting* findGMST(sol::object key) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			return nullptr;
		}

		auto index = -1;
		if (key.is<int>()) {
			index = key.as<int>();
		}
		else if (key.is<const char*>()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			auto& state = stateHandle.state;
			sol::object asIndex = state["tes3"]["gmst"][key.as<const char*>()];
			if (asIndex.is<int>()) {
				index = asIndex.as<int>();
			}
		}

		if (index >= TES3::GMST::sMonthMorningstar && index <= TES3::GMST::sWitchhunter) {
			return dataHandler->nonDynamicData->GMSTs[index];
		}

		return nullptr;
	}

	TES3::GameSetting* getGMST(sol::object key) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Display deprecation warning and traceback.
		logStackTrace("WARNING: Use of deprecated function tes3.getGMST. Use tes3.findGMST instead.");

		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			return nullptr;
		}

		if (key.is<int>()) {
			int index = key.as<int>();
			if (index >= TES3::GMST::sMonthMorningstar && index <= TES3::GMST::sWitchhunter) {
				return dataHandler->nonDynamicData->GMSTs[index];
			}
		}
		else if (key.is<std::string>()) {
			int index = -1;
			std::string keyStr = key.as<std::string>();
			for (int i = 0; i <= TES3::GMST::sWitchhunter; ++i) {
				TES3::GameSettingInfo* info = TES3::GameSettingInfo::get(i);
				if (strcmp(info->name, keyStr.c_str()) == 0) {
					index = i;
					break;
				}
			}

			if (index != -1) {
				return dataHandler->nonDynamicData->GMSTs[index];
			}
		}

		return nullptr;
	}

	bool playSound(sol::optional<sol::table> params) {
		// Get parameters.
		TES3::Sound* sound = getOptionalParamSound(params, "sound");
		const char* soundPath = getOptionalParam<const char*>(params, "soundPath", nullptr);
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		bool loop = getOptionalParam<bool>(params, "loop", false);
		int mix = getOptionalParam<int>(params, "mixChannel", int(TES3::AudioMixType::Effects));
		double volume = getOptionalParam<double>(params, "volume", 1.0);
		float pitch = getOptionalParam<float>(params, "pitch", 1.0f);

		if (sound == nullptr && soundPath == nullptr) {
			const auto id = getOptionalParam<const char*>(params, "sound", nullptr);
			if (id) {
				log::getLog() << "tes3.playSound: Could not locate sound with ID '" << id << "'." << std::endl;
				lua::logStackTrace();
			}
			return false;
		}

		// Clamp volume.
		volume = std::clamp(volume, 0.0, 1.0);

		// Apply mix and rescale to 0-250
		const auto worldController = TES3::WorldController::get();
		volume *= 250.0 * worldController->audioController->getMixVolume(TES3::AudioMixType(mix));

		// Only allow positional sounds if we are loaded in and have a player reference.
		const auto mobilePlayer = worldController->getMobilePlayer();
		if (mobilePlayer) {
			if (soundPath) {
				bool isVoiceover = getOptionalParam<bool>(params, "isVoiceover", false);
				TES3::DataHandler::get()->addTemporarySound(soundPath, reference, loop ? TES3::SoundPlayFlags::Loop : 0, int(volume), pitch, isVoiceover, sound);
				return true;
			}
			else if (sound) {
				TES3::DataHandler::get()->addSound(sound, reference, loop ? TES3::SoundPlayFlags::Loop : 0, int(volume), pitch);
				return true;
			}
		}

		// Try to fall back on direct-playing a sound.
		if (sound) {
			const auto flags = loop ? TES3::SoundPlayFlags::Loop : NULL;
			return sound->play(flags, volume, pitch, true);
		}

		return false;
	}

	bool getSoundPlaying(sol::optional<sol::table> params) {
		// Get parameters.
		TES3::Sound* sound = getOptionalParamSound(params, "sound");
		TES3::Reference* reference = getOptionalParamReference(params, "reference");

		if (sound == nullptr) {
			const auto id = getOptionalParam<const char*>(params, "sound", nullptr);
			if (id) {
				log::getLog() << "tes3.getSoundPlaying: Could not locate sound with ID '" << id << "'." << std::endl;
				lua::logStackTrace();
			}
			return false;
		}

		return bool(TES3::DataHandler::get()->getSoundPlaying(sound, reference));
	}

	void adjustSoundVolume(sol::optional<sol::table> params) {
		// Get parameters.
		TES3::Sound* sound = getOptionalParamSound(params, "sound");
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		auto mix = getOptionalParam<int>(params, "mixChannel", int(TES3::AudioMixType::Effects));
		auto volume = getOptionalParam<double>(params, "volume", 1.0);

		if (!sound || !reference) {
			log::getLog() << "tes3.adjustSoundVolume: Valid sound and reference required." << std::endl;
			return;
		}

		// Clamp volume.
		volume = std::max(0.0, volume);
		volume = std::min(volume, 1.0);

		// Apply mix and rescale to 0-250
		volume *= 250.0 * TES3::WorldController::get()->audioController->getMixVolume(TES3::AudioMixType(mix));

		TES3::DataHandler::get()->adjustSoundVolume(sound, reference, unsigned char(volume));
	}

	void removeSound(sol::optional<sol::table> params) {
		// Get parameters.
		TES3::Sound* sound = getOptionalParamSound(params, "sound");
		TES3::Reference* reference = getOptionalParamReference(params, "reference");

		TES3::DataHandler::get()->removeSound(sound, reference);
	}

	void streamMusic(sol::optional<sol::table> params) {
		auto worldController = TES3::WorldController::get();

		// Get parameters.
		const char* relativePath = getOptionalParam<const char*>(params, "path", nullptr);
		int situation = getOptionalParam<int>(params, "situation", int(TES3::MusicSituation::Uninterruptible));
		double crossfade = getOptionalParam<double>(params, "crossfade", 1.0);
		float volume = getOptionalParam<float>(params, "volume", worldController->audioController->getMusicVolume());

		if (relativePath) {
			char path[260];

			std::snprintf(path, sizeof(path), "Data Files/music/%s", relativePath);
			event::MusicChangeTrackEvent::ms_Context = "lua";
			worldController->musicSituation = TES3::MusicSituation(situation);
			worldController->audioController->changeMusicTrack(path, int(1000.0f * crossfade), volume);
		}
	}

	bool skipToNextMusicTrack(sol::optional<sol::table> params) {
		const auto worldController = TES3::WorldController::get();
		if (worldController == nullptr) {
			return false;
		}

		const auto audioController = worldController->audioController;
		if (audioController == nullptr) {
			return false;
		}

		const auto breakUninterruptibleMusic = getOptionalParam<bool>(params, "force", false);
		if (worldController->musicSituation == TES3::MusicSituation::Uninterruptible && !breakUninterruptibleMusic) {
			return false;
		}

		const auto mobilePlayer = worldController->getMobilePlayer();
		const auto defaultSituation = mobilePlayer && mobilePlayer->getFlagInCombat() ? TES3::MusicSituation::Combat : TES3::MusicSituation::Explore;
		const auto situation = (TES3::MusicSituation)getOptionalParam<int>(params, "situation", (int)defaultSituation);

		if (!worldController->selectNextMusicTrack(situation)) {
			return false;
		}

		const auto nextTrack = tes3::getThreadSafeStringBuffer();
		const auto crossfade = getOptionalParam<double>(params, "crossfade", 1.0);
		const auto volume = getOptionalParam<float>(params, "volume", audioController->getMusicVolume());

		audioController->changeMusicTrack(nextTrack, int(1000.0 * crossfade), volume);

		return true;
	}

	TES3::UI::Element* messageBox(sol::object param, sol::optional<sol::variadic_args> va) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		if (param.is<std::string>()) {
			std::string message = state["string"]["format"](param, va);
			return TES3::UI::showMessageBox(message.c_str());
		}
		else if (param.get_type() == sol::type::table) {
			sol::table params = param;

			// We need to make sure the strings stay in memory, we can't just snag the c-string in passing.
			std::string buttonText[32];
			struct {
				const char* text[32] = {};
			} buttonTextStruct;

			//  Get the parameters out of the table and into the table.
			std::string message = params["message"];
			sol::optional<sol::table> maybeButtons = params["buttons"];
			if (maybeButtons && maybeButtons.value().size() > 0) {
				sol::table buttons = maybeButtons.value();
				size_t size = std::min(buttons.size(), size_t(32));
				for (size_t i = 0; i < size; ++i) {
					std::string result = buttons[i + 1];
					if (result.empty()) {
						break;
					}

					buttonText[i] = result;
					buttonTextStruct.text[i] = buttonText[i].c_str();
				}
			}

			// No buttons, do a normal popup.
			else {
				auto showInDialog = getOptionalParam<bool>(params, "showInDialog", true);

				auto element = TES3::UI::showMessageBox(message.c_str(), nullptr, showInDialog);
				if (element) {
					// Allow overriding the duration.
					auto duration = getOptionalParam<float>(params, "duration");
					if (duration) {
						element->setProperty(TES3::UI::registerProperty("MenuNotify_timestamp"), duration.value());
					}
				}
				return element;
			}

			// Set up our event callback.
			LuaManager::getInstance().setButtonPressedCallback(params["callback"]);

			// Temporary hook into the function that creates message boxes.
			reinterpret_cast<void(__cdecl*)(const char*, ...)>(0x5F1AA0)(message.c_str(), buttonTextStruct, NULL);
			return TES3::UI::findMenu("MenuMessage");
		}
		else {
			sol::protected_function_result result = state["tostring"](param);
			if (result.valid()) {
				sol::optional<const char*> asString = result;
				if (asString) {
					return TES3::UI::showMessageBox(asString.value());
				}
			}
			throw std::exception("tes3.messageBox: Unable to convert parameter to string.");
		}
		return nullptr;
	}

	bool saveGame(sol::optional<sol::table> params) {
		const char* fileName = getOptionalParam<const char*>(params, "file", "quiksave");
		const char* saveName = getOptionalParam<const char*>(params, "name", "Quicksave");

		// Prevent saves from happening if the player is dead.
		if (TES3::WorldController::get()->getMobilePlayer()->health.current <= 0) {
			return false;
		}

		return TES3::DataHandler::get()->nonDynamicData->saveGame(fileName, saveName);
	}

	void loadGame(const char* fileName) {
		auto worldController = TES3::WorldController::get();
		// Char Gen State will equal 0 (NONE) in the menu.
		if (!worldController->isChargenFinished() && !worldController->isChargenRunning()) {
			TES3::DataHandler::get()->nonDynamicData->loadGameMainMenu(fileName);
		}
		else {
			TES3::DataHandler::get()->nonDynamicData->loadGame(fileName);
		}
	}

	void newGame() {
		auto cachedSuppressState = true;
		std::swap(TES3::DataHandler::suppressThreadLoad, cachedSuppressState);
		tes3::startNewGame();
		std::swap(TES3::DataHandler::suppressThreadLoad, cachedSuppressState);
	}

	bool isModActive(const char* modName) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler && dataHandler->nonDynamicData->activeModCount > 0) {
			// Check the active mod list if it is available.
			const auto activeMods = dataHandler->nonDynamicData->getActiveMods();
			for (auto gameFile : activeMods) {
				if (_strnicmp(gameFile->filename, modName, sizeof(gameFile->filename)) == 0) {
					return true;
				}
			}
		}
		else {
			// Otherwise we have to fall back to see if the mod will load.
			char buffer[512] = {};
			for (auto i = 0; i < 1024; ++i) {
				sprintf_s(buffer, "GameFile%d", i);
				if (!GetPrivateProfileStringA("Game Files", buffer, "", buffer, sizeof(buffer), ".\\Morrowind.ini")) {
					break;
				}

				if (string::iequal(buffer, modName)) {
					return true;
				}
			}
		}

		return false;
	}

	sol::object getModList() {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			return sol::nil;
		}

		auto activeMods = dataHandler->nonDynamicData->getActiveMods();
		sol::table mods = state.create_table();
		for (auto gameFile : activeMods) {
			mods.add(gameFile->filename);
		}

		return mods;
	}

	sol::table getArchiveList(sol::this_state s) {
		sol::state_view state = s;
		sol::table archives = state.create_table();

		for (auto archive = TES3::BSALoader::get()->lastLoadedArchive; archive; archive = archive->nextArchive) {
			archives.add(std::string(archive->path, strnlen_s(archive->path, 128)));
		}

		return archives;
	}

	TES3::QuickKey* getQuickKey(sol::table params) {
		auto slotNumber = getOptionalParam(params, "slot", 0) - 1;
		if (slotNumber < 0 || slotNumber > 8) {
			throw std::invalid_argument("Invalid 'slot' param provided.");
		}

		return TES3::QuickKey::getQuickKey(slotNumber);
	}

	void playItemPickupSound(sol::optional<sol::table> params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
		auto state = getOptionalParam<bool>(params, "pickup", true) ? TES3::ItemSoundState::Up : TES3::ItemSoundState::Down;
		if (item == NULL) {
			return;
		}

		TES3::WorldController::get()->playItemUpDownSound(item, state, reference);
	}

	auto iterateObjects(sol::optional<sol::object> filter) {
		std::unordered_set<unsigned int> filters;

		if (filter) {
			if (filter.value().is<unsigned int>()) {
				filters.insert(filter.value().as<unsigned int>());
			}
			else if (filter.value().is<sol::table>()) {
				sol::table filterTable = filter.value().as<sol::table>();
				for (const auto& kv : filterTable) {
					if (kv.second.is<unsigned int>()) {
						filters.insert(kv.second.as<unsigned int>());
					}
				}
			}
			else {
				throw std::invalid_argument("Iteration can only be filtered by object type, a table of object types, or must not have any filter.");
			}
		}

		return iterateObjectsFiltered(std::move(filters));
	}

	TES3::Sound* getSound(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return TES3::DataHandler::get()->nonDynamicData->findSound(id);
		}
		else {
			throw std::exception("Function called before Data Handler was initialized.");
		}
	}

	TES3::SoundGenerator* getSoundGenerator(std::string creatureId, unsigned int type) {
		auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;
		auto creature = nonDynamicData->resolveObjectByType<TES3::Creature>(creatureId);
		if (creature == nullptr) {
			return nullptr;
		}

		while (creature->soundGenerator) {
			creature = creature->soundGenerator;
		}

		auto soundGenerators = nonDynamicData->soundGenerators;
		const char* id = creature->getObjectID();
		size_t idLength = strnlen_s(id, 32);
		for (auto itt = soundGenerators->head; itt != nullptr; itt = itt->next) {
			if (itt->data->soundType != static_cast<TES3::SoundType>(type)) {
				continue;
			}

			if (_strnicmp(id, itt->data->name, idLength) == 0) {
				return itt->data;
			}
		}

		return nullptr;
	}

	TES3::Class* findClass(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->findClass(id);
		}
		return nullptr;
	}

	TES3::Race* findRace(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->findRace(id);
		}
		return nullptr;
	}

	TES3::Faction* getFaction(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->findFaction(id);
		}
		return nullptr;
	}

	TES3::MagicEffect* getMagicEffect(int id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			auto magicEffectsController = dataHandler->nonDynamicData->magicEffects;
			if (magicEffectsController->getEffectExists(id)) {
				return magicEffectsController->getEffectObject(id);
			}
		}
		return nullptr;
	}

	std::string getMagicEffectName(sol::table params) {
		// Try to get effect param.
		auto effect = getMagicEffect(getOptionalParam<int>(params, "effect", -1));
		if (!effect) {
			throw std::exception("Invalid 'effect' param provided.");
		}

		auto attribute = getOptionalParam<int>(params, "attribute", -1);
		auto skill = getOptionalParam<int>(params, "skill", -1);
		return std::move(effect->getComplexName(attribute, skill));
	}

	sol::optional<NI::Camera*> getCamera() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->worldCamera.cameraData.camera;
		}
		return sol::optional<NI::Camera*>();
	}

	sol::optional<TES3::Vector3> getCameraVector() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->worldCamera.cameraData.camera->worldDirection;
		}
		return sol::optional<TES3::Vector3>();
	}

	sol::optional<TES3::Vector3> getCameraPosition() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->worldCamera.cameraData.camera->worldBoundOrigin;
		}
		return sol::optional<TES3::Vector3>();
	}

	sol::optional<TES3::Vector3> getPlayerEyePosition() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			auto mobilePlayer = worldController->getMobilePlayer();
			if (mobilePlayer) {
				return mobilePlayer->animationController.asPlayer->firstPersonHeadCameraNode->worldTransform.translation;
			}
		}
		return sol::optional<TES3::Vector3>();
	}

	sol::optional<TES3::Vector3> getPlayerEyeVector() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			auto rotation = worldController->armCamera.cameraRoot->localRotation;
			return TES3::Vector3(rotation->m0.y, rotation->m1.y, rotation->m2.y);
		}
		return sol::optional<TES3::Vector3>();
	}

	int getPlayerActivationDistance() {
		int distance = 0;

		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			distance += dataHandler->nonDynamicData->GMSTs[TES3::GMST::iMaxActivateDist]->value.asLong;
		}

		auto worldController = TES3::WorldController::get();
		if (worldController) {
			auto macp = worldController->getMobilePlayer();
			if (macp) {
				distance += macp->telekinesis;
			}
		}

		return distance;
	}

	sol::optional<TES3::Vector3> get3rdPersonCameraOffset() {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			return {};
		}

		auto macp = worldController->getMobilePlayer();
		if (!macp) {
			return {};
		}

		return macp->animationController.asPlayer->cameraOffset;
	}

	void set3rdPersonCameraOffset(sol::table params) {
		auto offset = getOptionalParamVector3(params, "offset");
		if (!offset) {
			throw std::exception("Invalid 'offset' param provided.");
		}
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			throw std::exception("WorldController is not yet initialized.");
		}

		auto macp = worldController->getMobilePlayer();
		if (!macp) {
			throw std::exception("MobilePlayer is not yet initialized.");
		}

		macp->animationController.asPlayer->cameraOffset = offset.value();
	}

	const auto TES3_rand = reinterpret_cast<int(__cdecl*)(unsigned int)>(0x47B3B0);
	int random(sol::optional<sol::object> maybe_object) {
		if (maybe_object) {
			sol::object object = maybe_object.value();
			if (object.is<void*>()) {
				return TES3_rand(unsigned int(object.as<void*>()));
			}
			else if (object.is<double>()) {
				return TES3_rand(unsigned int(object.as<double>()));
			}
			else {
				throw std::exception("Object must be reasonably convertable to a number.");
			}
		}
		else {
			return TES3_rand(0);
		}
	}

	static NI::Pick* rayTestCache = nullptr;
	static std::vector<NI::AVObject*> rayTestIgnoreRoots;
	sol::object rayTest(sol::table params, sol::this_state this_state) {
		sol::state_view state = this_state;

		// Make sure we got our required position.
		sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
		if (!position) {
			return sol::make_object(state, false);
		}

		// Make sure we got our required direction.
		sol::optional<TES3::Vector3> direction = getOptionalParamVector3(params, "direction");
		if (!direction) {
			return sol::make_object(state, false);
		}

		// Get optional maximum search distance.
		auto maxDistance = getOptionalParam<float>(params, "maxDistance", 0.0f);

		// Create our pick if it doesn't exist. If it does exist, clear all results.
		if (rayTestCache == nullptr) {
			rayTestCache = NI::Pick::malloc();
		}
		else {
			rayTestCache->clearResults();
		}

		// Added ablity to use any node
		// In Lua Script use "root = tes3.mobilePlayer.firstPersonReference.sceneNode"
		// to have rayTest scan 1st person scene node
		rayTestCache->root = getOptionalParam<NI::Node*>(params, "root", TES3::Game::get()->worldRoot);

		// Are we finding all or the first?
		if (getOptionalParam<bool>(params, "findAll", false)) {
			rayTestCache->pickType = NI::PickType::FIND_ALL;
		}
		else {
			rayTestCache->pickType = NI::PickType::FIND_FIRST;
		}

		// Sort results by distance?
		if (getOptionalParam<bool>(params, "sort", true)) {
			rayTestCache->sortType = NI::PickSortType::SORT;
		}
		else {
			rayTestCache->sortType = NI::PickSortType::NO_SORT;
		}

		// Use triangle or model bounds for intersection?
		if (getOptionalParam<bool>(params, "useModelBounds", false)) {
			rayTestCache->intersectType = NI::PickIntersectType::BOUND_INTERSECT;
		}
		else {
			rayTestCache->intersectType = NI::PickIntersectType::TRIANGLE_INTERSECT;
		}

		// Use model coordinates or world coordinates?
		if (getOptionalParam<bool>(params, "useModelCoordinates", false)) {
			rayTestCache->coordinateType = NI::PickCoordinateType::MODEL_COORDINATES;
		}
		else {
			rayTestCache->coordinateType = NI::PickCoordinateType::WORLD_COORDINATES;
		}

		// Use the back side of a triangle? Note: Parameter name flipped!
		rayTestCache->frontOnly = !getOptionalParam<bool>(params, "useBackTriangles", false);

		// Observe app cull flag?
		rayTestCache->observeAppCullFlag = getOptionalParam<bool>(params, "observeAppCullFlag", true);

		// Determine what returned values we care about.
		rayTestCache->returnColor = getOptionalParam<bool>(params, "returnColor", false);
		rayTestCache->returnNormal = getOptionalParam<bool>(params, "returnNormal", false);
		rayTestCache->returnSmoothNormal = getOptionalParam<bool>(params, "returnSmoothNormal", false);
		rayTestCache->returnTexture = getOptionalParam<bool>(params, "returnTexture", false);

		// Default root nodes to ignore.
		if (rayTestIgnoreRoots.empty()) {
			// Initialize rayTestIgnoreRoots on first use.
			auto worldController = TES3::WorldController::get();
			auto weather = worldController->weatherController;

			rayTestIgnoreRoots.push_back(weather->sgRainRoot);
			rayTestIgnoreRoots.push_back(weather->sgSnowRoot);
			rayTestIgnoreRoots.push_back(weather->sgStormRoot);
			rayTestIgnoreRoots.push_back(worldController->mobManager->projectileManager->worldProjectileRoot);
			rayTestIgnoreRoots.push_back(worldController->magicInstanceController->worldSpellRoot);
			rayTestIgnoreRoots.push_back(worldController->vfxManager->worldVFXRoot);
		}

		std::vector<NI::AVObject*> ignoreRestoreList;
		for (const auto node : rayTestIgnoreRoots) {
			if (!node->getAppCulled()) {
				node->setAppCulled(true);
				ignoreRestoreList.push_back(node);
			}
		}

		// Allow defining references/nodes to ignore from the raytest.
		sol::optional<sol::table> ignoreTable = params["ignore"];
		if (ignoreTable) {
			for (const auto& kvPair : ignoreTable.value()) {
				sol::object value = kvPair.second;
				if (value.is<NI::Node>()) {
					auto node = value.as<NI::Node*>();
					if (!node->getAppCulled()) {
						// Cull the node, and add it to a list to set as unculled later.
						node->setAppCulled(true);
						ignoreRestoreList.push_back(node);
					}
				}
				else if (value.is<TES3::Reference>()) {
					auto reference = value.as<TES3::Reference*>();
					if (reference->sceneNode) {
						if (!reference->sceneNode->getAppCulled()) {
							// Cull the node, and add it to a list to set as unculled later.
							reference->sceneNode->setAppCulled(true);
							ignoreRestoreList.push_back(reference->sceneNode);
						}
					}
				}
				else {
					// Restore previous cull states.
					for (const auto node : ignoreRestoreList) {
						node->setAppCulled(false);
					}

					throw std::exception("tes3.rayTest: Invalid item in ignore list. Must contain only scene graph nodes or references.");
				}
			}
		}

		// Determine if we are doing an accurate test.
		const auto accurateSkinned = getOptionalParam<bool>(params, "accurateSkinned", false);
		const auto existingUseNewSkinning = Configuration::UseSkinnedAccurateActivationRaytests;
		Configuration::UseSkinnedAccurateActivationRaytests = accurateSkinned;

		// Our pick is configured. Let's run it!
		const auto directionNormalized = direction.value().normalized();
		const auto pickSuccess = rayTestCache->pickObjects(&position.value(), &directionNormalized, false, maxDistance);
		Configuration::UseSkinnedAccurateActivationRaytests = existingUseNewSkinning;

		// Restore previous cull states.
		for (auto itt = ignoreRestoreList.begin(); itt != ignoreRestoreList.end(); ++itt) {
			(*itt)->setAppCulled(false);
		}

		// Did we get any results?
		if (!pickSuccess || rayTestCache->results.empty()) {
			return sol::nil;
		}

		// Fix distances and missing intersection data for skinned nodes.
		if (!accurateSkinned) {
			const auto distanceScale = 1.0f / direction.value().length();
			const auto skinnedCorrection = (maxDistance != 0.0f) ? maxDistance : 1.0f;

			for (const auto& r : rayTestCache->results) {
				if (r == nullptr) {
					continue;
				}

				// Adjust distance as if direction was not normalized.
				r->distance *= distanceScale;

				// Skinned nodes only have usable scaled distance data.
				if (r->object->isInstanceOfType(NI::RTTIStaticPtr::NiTriBasedGeom)) {
					const auto geometry = static_cast<const NI::TriBasedGeometry*>(r->object.get());
					if (geometry->skinInstance) {
						r->distance *= skinnedCorrection;
						r->intersection = position.value() + direction.value() * r->distance;
						r->normal = (r->intersection - geometry->worldBoundOrigin).normalized();
					}
				}
			}
		}

		// Are we looking for a single result?
		if (rayTestCache->pickType == NI::PickType::FIND_FIRST) {
			return sol::make_object(state, rayTestCache->results[0]);
		}

		// We're now in multi-result mode. We'll store these in a table.
		sol::table results = state.create_table();

		// Go through and clone the results in a way that will play nice.
		for (auto& r : rayTestCache->results) {
			results.add(r);
		}

		return results;
	}

	bool isThirdPerson() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			auto macp = worldController->getMobilePlayer();
			if (macp) {
				return macp->is3rdPerson();
			}
		}
		return false;
	}

	void tapKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGETapKey);
	}

	void pushKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEPushKey);
	}

	void releaseKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEReleaseKey);
	}

	void hammerKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEHammerKey);
	}

	void unhammerKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEUnhammerKey);
	}

	void enableKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEAllowKey);
	}

	void disableKey(int key) {
		Stack::getInstance().pushLong(key);
		mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEDisallowKey);
	}

	sol::optional<bool> hasCodePatchFeature(int id) {
		if (!mcp::hasFeaturesFound()) {
			return sol::optional<bool>();
		}
		return mcp::getFeatureEnabled(id);
	}

	sol::optional<int> getDaysInMonth(int month) {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->getDaysInMonth(month);
		}
		return sol::optional<int>();
	}

	sol::optional<int> getCumulativeDaysForMonth(int month) {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->getCumulativeDaysForMonth(month);
		}
		return sol::optional<int>();
	}

	double getSimulationTimestamp(sol::optional<bool> highPrecision) {
		if (highPrecision.value_or(true)) {
			auto worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->getHighPrecisionSimulationTimestamp();
			}
		}
		else {
			return mwse::tes3::getSimulationTimestamp();
		}
		return 0.0;
	}

	TES3::EquipmentStack* getEquippedItem(sol::table params) {
		// Find our equipment based on the object given.
		TES3::IteratedList<TES3::EquipmentStack*>* equipment = NULL;
		sol::object actor = params["actor"];
		if (actor.valid()) {
			if (actor.is<TES3::Reference*>()) {
				equipment = actor.as<TES3::Reference*>()->getEquipment();
			}
			else if (actor.is<TES3::MobileActor*>()) {
				equipment = actor.as<TES3::MobileActor*>()->reference->getEquipment();
			}
			else if (actor.is<TES3::Actor*>()) {
				equipment = &actor.as<TES3::Actor*>()->equipment;
			}
		}
		else {
			throw std::exception("No actor provided.");
		}

		// Make sure we got the equipment.
		if (equipment == nullptr) {
			throw std::exception("The provided actor's equipment could not be resolved.");
		}

		// Get filter: Item Type
		sol::optional<int> filterObjectType;
		sol::optional<int> filterSlot;
		if (params["objectType"].valid()) {
			filterObjectType = (int)params["objectType"];

			// Get filter: Item Slot/Type
			if (params["slot"].valid()) {
				filterSlot = (int)params["slot"];
			}
			else if (params["type"].valid()) {
				filterSlot = (int)params["type"];
			}
		}

		// Get filter: Item Enchanted
		sol::optional<bool> filterEnchanted;
		if (params["enchanted"].valid()) {
			filterEnchanted = (bool)params["enchanted"];
		}

		// Loop through the items and pick the first one that matches our filters.
		for (auto itt = equipment->head; itt != nullptr; itt = itt->next) {
			TES3::Object* object = itt->data->object;

			// Filter object type.
			if (filterObjectType && filterObjectType.value() != object->objectType) {
				continue;
			}

			// Filter slot/type.
			if (filterSlot && filterSlot.value() != object->getType()) {
				continue;
			}

			// Filter enchanted.
			if (filterEnchanted) {
				TES3::Enchantment* enchantment = object->getEnchantment();
				if (filterEnchanted.value() == true && enchantment == nullptr) {
					continue;
				}
				else if (filterEnchanted.value() == false && enchantment != nullptr) {
					continue;
				}
			}

			// If we got this far we match all filters. Return the object.
			return itt->data;
		}

		return nullptr;
	}

	TES3::InputConfig* getInputBinding(int code) {
		if (code < TES3::KeyBind::FirstKey || code > TES3::KeyBind::LastKey) {
			return nullptr;
		}

		auto worldController = TES3::WorldController::get();
		if (worldController) {
			TES3::InputController* inputController = worldController->inputController;
			if (inputController) {
				return &inputController->inputMaps[code];
			}
		}
		return nullptr;
	}

	TES3::Region* getRegion(sol::optional<sol::table> maybeParams) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			// Try to get the current cell's region first.
			if (dataHandler->currentCell) {
				TES3::Region* region = dataHandler->currentCell->getRegion();
				if (region) {
					return region;
				}

				// Try to get a cell region.
				if (dataHandler->currentCell->getIsInterior() && getOptionalParam(maybeParams, "useDoors", false)) {
					for (auto ref : dataHandler->currentCell->persistentRefs) {
						if (ref->baseObject->objectType == TES3::ObjectType::Door) {
							auto destination = ref->getAttachedTravelDestination();
							if (destination && destination->cell) {
								auto region = destination->cell->getRegion();
								if (region) {
									return region;
								}
							}
						}
					}
				}
			}

			// Otherwise fall back to the last exterior cell's region.
			if (dataHandler->lastExteriorCell) {
				return dataHandler->lastExteriorCell->getRegion();
			}
		}

		return nullptr;
	}

	TES3::Region* findRegion(const char* id) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return dataHandler->nonDynamicData->getRegion(id);
		}
		return nullptr;
	}

	TES3::Weather* getCurrentWeather() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return worldController->weatherController->currentWeather;
		}
		return nullptr;
	}

	void changeWeather(sol::table params) {
		auto weatherId = getOptionalParam<int>(params, "id");
		auto immediate = getOptionalParam<bool>(params, "immediate", false);

		if (!weatherId) {
			throw std::invalid_argument("tes3.changeWeather: id param is required.");
		}

		auto weatherController = TES3::WorldController::get()->weatherController;
		if (immediate) {
			weatherController->switchImmediate(weatherId.value());
		}
		else {
			weatherController->switchTransition(weatherId.value());
		}
	}

	sol::optional<TES3::Vector2> getCursorPosition() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			return TES3::Vector2(worldController->mouseController->position.x, worldController->mouseController->position.z);
		}
		return sol::optional<TES3::Vector2>();
	}

	TES3::Skill* getSkill(int skillID) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			return &dataHandler->nonDynamicData->skills[skillID];
		}
		return nullptr;
	}

	void removeEffects(sol::table params) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::exception("tes3.removeEffects: No reference parameter provided.");
		}

		int effect = getOptionalParam<int>(params, "effect", -1);
		int castType = getOptionalParam<int>(params, "castType", -1);
		int chance = getOptionalParam<int>(params, "chance", 100);
		if (chance > 100) {
			chance = 100;
		}
		else if (chance < 0) {
			chance = 0;
		}

		if (effect != -1) {
			TES3::WorldController::get()->magicInstanceController->removeSpellsByEffect(reference, effect, chance);
		}
		else if (castType != -1) {
			bool removeSpell = getOptionalParam<bool>(params, "removeSpell", castType != int(TES3::SpellCastType::Spell));
			TES3::WorldController::get()->magicInstanceController->clearSpellEffect(reference, castType, chance, removeSpell);
		}
		else {
			throw std::exception("tes3.removeEffects: Must pass either 'effect' or 'castType' parameter!");
		}
	}

	int getPlayerGold() {
		auto worldController = TES3::WorldController::get();
		if (worldController) {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (macp) {
				return macp->getGold();
			}
		}
		return 0;
	}

	bool triggerCrime(sol::table params) {
		TES3::CrimeEvent crimeEvent;

		// Verify the provided type.
		int crimeType = getOptionalParam<int>(params, "type", TES3::CrimeType::Theft);
		if (crimeType < TES3::CrimeType::Attack || crimeType > TES3::CrimeType::Werewolf) {
			throw std::invalid_argument("Invalid 'type' parameter provided. Must be one of the values in the 'tes3.crimeType' table.");
		}
		crimeEvent.type = crimeType;

		// The player is always the criminal. Other values will have no effect and do not get reacted to by AI actors.
		crimeEvent.criminal = TES3::WorldController::get()->getMobilePlayer();

		// Set some basic crime event data.
		crimeEvent.timestamp = float(timeGetTime());
		crimeEvent.position = crimeEvent.criminal->reference->position;
		int stolenValue = getOptionalParam<int>(params, "value", crimeEvent.stolenValue);

		// Try to set as many victim-related crime event fields as possible.
		sol::object victim = params["victim"];
		crimeEvent.victim = crimeEvent.criminal;
		if (victim.is<TES3::Faction>()) {
			crimeEvent.victimFaction = victim.as<TES3::Faction*>();
		}
		else if (victim.is<TES3::Actor>()) {
			crimeEvent.victim = static_cast<TES3::MobileActor*>(victim.as<TES3::Actor*>()->getMobile());
			crimeEvent.victimFaction = victim.as<TES3::Actor*>()->getFaction();
			if (victim.is<TES3::NPC>()) {
				crimeEvent.stolenFrom = victim.as<TES3::NPC*>();
			}
			else if (victim.is<TES3::NPCInstance>()) {
				crimeEvent.stolenFrom = victim.as<TES3::NPCInstance*>()->baseNPC;
			}
		}
		else if (victim.is<TES3::MobileNPC>()) {
			TES3::MobileNPC* mobileNPC = victim.as<TES3::MobileNPC*>();
			crimeEvent.victim = mobileNPC;
			crimeEvent.victimFaction = mobileNPC->npcInstance->getFaction();
			crimeEvent.stolenFrom = mobileNPC->npcInstance;
		}

		// Set crime event data based on the crime committed.
		bool isAttack = false;
		switch (crimeType) {
		case TES3::CrimeType::Attack:
			isAttack = true;
			crimeEvent.bountyKey = "attack";
			crimeEvent.stolenFrom = nullptr;
			break;
		case TES3::CrimeType::Killing:
			isAttack = true;
			crimeEvent.bountyKey = "killing";
			crimeEvent.stolenFrom = nullptr;
			break;
		case TES3::CrimeType::Pickpocket:
			crimeEvent.bountyKey = "pickpocket";
			crimeEvent.stolenValue = 25;
			break;
		case TES3::CrimeType::WitnessReaction: // Not really a valid type for this function, but redirected to TES3::CrimeType::Theft for backwards compatibility.
		case TES3::CrimeType::Theft:
			crimeEvent.type = TES3::CrimeType::Theft;
			crimeEvent.bountyKey = "theft";
			crimeEvent.stolenValue = stolenValue;
			break;
		case TES3::CrimeType::Trespass:
			crimeEvent.bountyKey = "trespass";
			crimeEvent.victim = crimeEvent.criminal; // The player mobile will always be the victim for this crime type.
			crimeEvent.stolenFrom = nullptr;
			break;
		case TES3::CrimeType::Werewolf:
			crimeEvent.bountyKey = "werewolf";
			crimeEvent.victim = crimeEvent.criminal; // The player mobile will always be the victim for this crime type.
			crimeEvent.stolenFrom = nullptr;
			break;
		}

		// Try to detect the crime.
		bool forceDetection = getOptionalParam<bool>(params, "forceDetection", false);
		auto processManager = TES3::WorldController::get()->mobManager->processManager;
		if (forceDetection || (isAttack && processManager->detectAttack(crimeEvent.criminal)) || (!isAttack && processManager->detectPresence(crimeEvent.criminal))) {
			processManager->checkAlarmRadius(crimeEvent.criminal, crimeEvent.witnesses);
		}

		// Add the crime to the criminal's committed crimes list if there are witnesses.
		if (crimeEvent.witnesses->count) {
			crimeEvent.criminal->committedCrimes.push_front(crimeEvent);
			return true;
		}

		return false;
	}

	NI::Pointer<NI::Node> loadMesh(const char* relativePath, sol::optional<bool> useCached) {
		if (relativePath == nullptr) {
			throw std::invalid_argument("No path provided. Pass the path relative to the Meshes folder as the first parameter.");
		}

		std::string path = "Meshes\\";
		path += relativePath;

		auto meshData = TES3::DataHandler::get()->nonDynamicData->meshData;
		if (useCached.value_or(true)) {
			return meshData->loadMesh(path.c_str());
		}
		else {
			// Create a non-cached version by calling the constructors manually without the usual checking.
			return meshData->loadMeshUncached(path.c_str());
		}

		return nullptr;
	}

	NI::Pointer<NI::SourceTexture> loadSourceTexture(const char* relativePath, sol::optional<bool> useCached) {
		std::string path = "Textures\\";
		path += relativePath;

		if (useCached.value_or(true)) {
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				throw std::runtime_error("tes3dataHandler is not yet initialized.");
			}
			return dataHandler->loadSourceTexture(path.c_str());
		}
		else {
			return NI::SourceTexture::createFromPath(path.c_str());
		}
	}

	bool playVoiceover(sol::table params) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Get the actor that we're going to make say something.
		auto actor = getOptionalParamMobileActor(params, "actor");
		if (actor == nullptr) {
			return false;
		}

		// Accept either a string or a number as the voiceover id.
		int voiceover = -1;
		sol::object voiceoverObject = params["voiceover"];
		if (voiceoverObject.is<const char*>()) {
			sol::object result = state["tes3"]["voiceover"][voiceoverObject.as<const char*>()];
			if (result.is<int>()) {
				voiceover = result.as<int>();
			}
		}
		else if (voiceoverObject.is<int>()) {
			voiceover = voiceoverObject.as<int>();
		}

		// Validate the input.
		if (voiceover < TES3::Voiceover::First || voiceover > TES3::Voiceover::Last) {
			return false;
		}

		actor->playVoiceover(voiceover);
		return true;
	}

	bool getLocked(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return false;
		}

		auto node = reference->getAttachedLockNode();
		if (node == nullptr) {
			return false;
		}

		return node->locked;
	}

	bool setLockLevel(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return false;
		}

		auto node = reference->getOrCreateLockNode();
		if (node == nullptr) {
			return false;
		}

		int level = getOptionalParam<int>(params, "level", -1);
		if (level >= 0) {
			node->lockLevel = level;
			reference->setObjectModified(true);
			return true;
		}

		return false;
	}

	sol::optional<int> getLockLevel(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return sol::optional<int>();
		}

		auto node = reference->getAttachedLockNode();
		if (node == nullptr) {
			return sol::optional<int>();
		}

		return node->lockLevel;
	}

	bool lock(sol::table params) {
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return false;
		}

		// Manage state for doors.
		if (reference->baseObject->objectType == TES3::ObjectType::Door) {
			// Clear door opening/closing flags.
			reference->clearActionFlag(TES3::ActionFlags::DoorOpening | TES3::ActionFlags::DoorClosing | TES3::ActionFlags::DoorJammedOpening | TES3::ActionFlags::DoorJammedClosing);

			// Reset orientation.
			auto orientationAttachment = static_cast<TES3::NewOrientationAttachment*>(reference->getAttachment(TES3::AttachmentType::NewOrientation));
			if (orientationAttachment) {
				orientationAttachment->orientation.z = reference->orientation.z;
			}

			// Update the scene graph with the new orientation.
			if (reference->sceneNode) {
				TES3::Matrix33 tempOutArg;
				reference->sceneNode->setLocalRotationMatrix(reference->updateSceneMatrix(&tempOutArg));
				reference->sceneNode->update();
			}
		}

		// Get or create our lock node.
		auto lockNode = reference->getOrCreateLockNode();
		if (lockNode == nullptr) {
			return false;
		}

		// Set the lock level if one was provided.
		int lockLevel = getOptionalParam<int>(params, "level", -1);
		if (lockLevel >= 0) {
			lockNode->lockLevel = lockLevel;
		}

		// Set the locked state, and return true if it changed.
		reference->setObjectModified(true);
		if (!lockNode->locked) {
			lockNode->locked = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool unlock(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return false;
		}

		auto node = reference->getAttachedLockNode();
		if (node == nullptr) {
			return false;
		}

		if (node->locked) {
			node->locked = false;
			reference->setObjectModified(true);
			return true;
		}
		else {
			return false;
		}
	}

	TES3::Spell* getTrap(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return nullptr;
		}

		auto node = reference->getAttachedLockNode();
		if (node == nullptr) {
			return nullptr;
		}

		return node->trap;
	}

	bool setTrap(sol::table params) {
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			return false;
		}

		// Only accept containers or doors.
		auto objectType = reference->baseObject->objectType;
		if (objectType != TES3::ObjectType::Container && objectType != TES3::ObjectType::Door) {
			return false;
		}

		// Get or create our lock node.
		auto lockNode = reference->getOrCreateLockNode();
		if (lockNode == nullptr) {
			return false;
		}

		lockNode->trap = getOptionalParamSpell(params, "spell");
		reference->setObjectModified(true);
		return true;
	}

	int getValue(sol::table params) {
		auto item = getOptionalParamObject<TES3::Item>(params, "item");
		auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);

		// If we weren't given an item, see if we were given a reference.
		if (item == nullptr) {
			auto reference = getOptionalParamExecutionReference(params);
			if (reference) {
				if (!reference->baseObject->isItem()) {
					throw std::invalid_argument("Invalid reference parameter provided: The reference is not an item.");
				}
				item = static_cast<TES3::Item*>(reference->baseObject);
				itemData = reference->getAttachedItemData();
			}
		}

		// Were we given valid data?
		if (item == nullptr) {
			throw std::invalid_argument("Invalid parameters provided: Must pass either an 'item' parameter or a 'reference' parameter.");
		}
		else if (!item->isItem()) {
			throw std::invalid_argument("Invalid item parameter provided: The object is not an item.");
		}

		auto useSoulValue = getOptionalParam(params, "useSoulValue", true);
		auto useDurability = getOptionalParam(params, "useDurability", true);
		return item->getBaseBarterValue(itemData, useSoulValue, useDurability);
	}

	bool checkMerchantTradesItem(sol::table params) {
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided: Can't be nil.");
		}

		TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid item parameter provided: Can't be nil.");
		}

		auto actor = reinterpret_cast<TES3::Actor*>(reference->baseObject);
		if (!actor->getAIConfig()) {
			throw std::invalid_argument("Invalid reference parameter provided: Base object must be an actor.");
		}

		return actor->tradesItemType(item->objectType);
	}

	std::pair<bool, TES3::DialogueInfo*> checkMerchantOffersService(sol::table params) {
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided: Can't be nil.");
		}

		auto actor = reinterpret_cast<TES3::Actor*>(reference->baseObject);
		if (!actor->isActor()) {
			throw std::invalid_argument("Invalid reference parameter provided: Base object must be an actor.");
		}

		// Check if the actor is offering the specific service.
		sol::optional<int> service = params["service"];
		if (service && !actor->offersService(service.value())) {
			return { false, nullptr };
		}

		// Allow overriding the context.
		const auto context = (TES3::Dialogue::GetFilteredInfoContext)getOptionalParam(params, "context", (int)TES3::Dialogue::GetFilteredInfoContext::Script);

		// Check for service refusal response.
		const int serviceRefusalPage = 7;
		auto dialogue = TES3::Dialogue::getDialogue((int)TES3::DialogueType::Persuasion, serviceRefusalPage);
		auto serviceRefusal = dialogue->getFilteredInfoWithContext(actor, reference, true, context);

		return { serviceRefusal == nullptr, serviceRefusal };
	}

	sol::optional<int> getJournalIndex(sol::table params) {
		TES3::Dialogue* journal = getOptionalParamDialogue(params, "id");
		if (journal == nullptr || journal->type != TES3::DialogueType::Journal) {
			return sol::optional<int>();
		}

		return journal->journalIndex;
	}

	bool setJournalIndex(sol::table params) {
		TES3::Dialogue* journal = getOptionalParamDialogue(params, "id");
		if (journal == nullptr || journal->type != TES3::DialogueType::Journal) {
			return false;
		}

		sol::optional<int> index = params["index"];
		if (!index) {
			return false;
		}

		if (!journal->setJournalIndexAndMarkModified(index.value())) {
			return false;
		}

		sol::optional<bool> showMessage = params["showMessage"];
		if (showMessage.value_or(false) && tes3::ui::getMenuNode(*reinterpret_cast<short*>(0x7D3442)) == nullptr) {
			TES3::UI::showMessageBox(TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sJournalEntry]->value.asString);
		}

		return true;
	}

	bool updateJournal(sol::table params) {
		TES3::Dialogue* journal = getOptionalParamDialogue(params, "id");
		if (journal == nullptr || journal->type != TES3::DialogueType::Journal) {
			return false;
		}

		sol::optional<int> index = params["index"];
		if (!index) {
			return false;
		}

		TES3::MobileActor* actor = getOptionalParamMobileActor(params, "speaker");
		if (actor == nullptr) {
			actor = TES3::WorldController::get()->getMobilePlayer();
		}

		if (!journal->addToJournal(index.value(), actor)) {
			return false;
		}

		sol::optional<bool> showMessage = params["showMessage"];
		if (showMessage.value_or(true) && tes3::ui::getMenuNode(*reinterpret_cast<short*>(0x7D3442)) == nullptr) {
			TES3::UI::showMessageBox(TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sJournalEntry]->value.asString);
		}

		return true;
	}

	void addJournalEntry(sol::table params) {
		auto worldController = TES3::WorldController::get();
		if (worldController == nullptr) {
			throw std::runtime_error("Function called before world controller is loaded.");
		}

		auto journalHTML = worldController->journalHTML;
		if (journalHTML == nullptr || journalHTML->data == nullptr) {
			throw std::runtime_error("No current journal data exists.");
		}

		auto text = getOptionalParam<const char*>(params, "text", nullptr);
		if (text == nullptr) {
			throw std::invalid_argument("Invalid 'text' parameter provided.");
		}

		journalHTML->writeTimestampedEntry(text);

		auto showMessage = getOptionalParam(params, "showMessage", true);
		if (showMessage) {
			journalHTML->showJournalUpdateNotification();
		}
	}

	bool getFileExists(std::string path) {
		// Sanitize path.
		std::replace(path.begin(), path.end(), '/', '\\');

		return tes3::resolveAssetPath(path.c_str()) != TES3::FileLoadSource::Missing;
	}

	sol::optional<std::tuple<std::string, std::string>> getFileSource(std::string path) {
		// Sanitize path.
		std::replace(path.begin(), path.end(), '/', '\\');

		char buffer[512];
		int result = tes3::resolveAssetPath(path.c_str(), buffer);

		switch (result) {
		case TES3::FileLoadSource::File:
			return { { "file", buffer } };
		case TES3::FileLoadSource::BSA:
			return { { "bsa", buffer } };
		}

		return {};
	}

	// Very slow method to get an INFO record by its ID.
	TES3::DialogueInfo* getDialogueInfo(sol::table params) {
		TES3::Dialogue* dialogue = getOptionalParamDialogue(params, "dialogue");
		const char* id = getOptionalParam<const char*>(params, "id", nullptr);
		if (dialogue == nullptr || id == nullptr) {
			return nullptr;
		}

		for (auto itt = dialogue->info.head; itt; itt = itt->next) {
			auto dialogueInfo = itt->data;
			if (!dialogueInfo->loadId()) {
				continue;
			}

			if (_strcmpi(id, dialogueInfo->loadLinkNode->name) == 0) {
				dialogueInfo->unloadId();
				return dialogueInfo;
			}

			dialogueInfo->unloadId();
		}

		return nullptr;
	}

	TES3::Cell* getCell(sol::table params) {
		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			throw std::runtime_error("Cell data has not yet been initialized.");
		}

		// If we were given a name, try that.
		const auto id = getOptionalParam<const char*>(params, "id");
		if (id) {
			return dataHandler->nonDynamicData->getCellByName(id.value());
		}

		// If we were given x/y coordinates use that.
		const auto gridX = getOptionalParam<int>(params, "x");
		const auto gridY = getOptionalParam<int>(params, "y");
		if (gridX && gridY) {
			return dataHandler->nonDynamicData->getCellByGrid(gridX.value(), gridY.value());
		}

		// If we were given a position, try that.
		const auto position = getOptionalParamVector3(params, "position");
		if (position) {
			const auto gridX = TES3::Cell::toGridCoord(position.value().x);
			const auto gridY = TES3::Cell::toGridCoord(position.value().y);
			return dataHandler->nonDynamicData->getCellByGrid(gridX, gridY);
		}

		// Incorrect call made. Throw an error.
		throw std::invalid_argument("Invalid call. Must provide an id, position, or grid X and Y coordinates.");
	}

	void fadeIn(sol::optional<sol::table> params) {
		TES3::Fader* fader = getOptionalParam(params, "fader", TES3::WorldController::get()->transitionFader);
		if (fader == nullptr) {
			return;
		}

		float duration = getOptionalParam(params, "duration", 1.0f);
		fader->fadeTo(0.0f, duration);
	}

	void fadeOut(sol::optional<sol::table> params) {
		TES3::Fader* fader = getOptionalParam(params, "fader", TES3::WorldController::get()->transitionFader);
		if (fader == nullptr) {
			return;
		}

		float duration = getOptionalParam(params, "duration", 1.0f);
		fader->fadeTo(1.0f, duration);
	}

	void fadeTo(sol::optional<sol::table> params) {
		TES3::Fader* fader = getOptionalParam(params, "fader", TES3::WorldController::get()->transitionFader);
		if (fader == nullptr) {
			return;
		}

		float value = getOptionalParam(params, "value", 1.0f);
		float duration = getOptionalParam(params, "duration", 1.0f);
		fader->fadeTo(value, duration);
	}

	void setStatistic(sol::table params) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Figure out our mobile object, in case someone gives us a reference instead.
		sol::userdata maybeMobile = params["reference"];
		if (maybeMobile.is<TES3::Reference>()) {
			maybeMobile = maybeMobile["mobile"];
		}

		// Make sure our object is of the right type.
		if (!maybeMobile.is<TES3::MobileActor>()) {
			throw std::invalid_argument("tes3.setStatistic: Could not resolve parameter 'reference'.");
		}

		// Try to get our statistic.
		TES3::MobileActor* mobile = maybeMobile.as<TES3::MobileActor*>();
		TES3::Statistic* statistic = nullptr;
		sol::optional<const char*> statisticName = params["name"];
		sol::optional<int> statisticSkill = params["skill"];
		sol::optional<int> statisticAttribute = params["attribute"];
		if (statisticSkill) {
			if (mobile->actorType == TES3::MobileActorType::Creature) {
				if (statisticSkill.value() >= TES3::CreatureSkillID::FirstSkill && statisticSkill.value() <= TES3::CreatureSkillID::LastSkill) {
					statistic = &static_cast<TES3::MobileCreature*>(mobile)->skills[statisticSkill.value()];
				}
				else {
					std::stringstream ss;
					ss << "tes3.setStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for creature.";
					throw std::invalid_argument(ss.str());
				}
			}
			else {
				if (statisticSkill.value() >= TES3::SkillID::FirstSkill && statisticSkill.value() <= TES3::SkillID::LastSkill) {
					statistic = &static_cast<TES3::MobileNPC*>(mobile)->skills[statisticSkill.value()];
				}
				else {
					std::stringstream ss;
					ss << "tes3.setStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for NPC.";
					throw std::invalid_argument(ss.str());
				}
			}
		}
		else if (statisticAttribute) {
			if (statisticAttribute.value() >= TES3::Attribute::FirstAttribute && statisticAttribute.value() <= TES3::Attribute::LastAttribute) {
				statistic = &mobile->attributes[statisticAttribute.value()];
			}
			else {
				std::stringstream ss;
				ss << "tes3.setStatistic: Invalid attribute index " << std::dec << statisticSkill.value() << ".";
				throw std::invalid_argument(ss.str());
			}
		}
		else if (statisticName) {
			sol::optional<TES3::Statistic*> maybeStatistic = maybeMobile[statisticName.value()];
			if (maybeStatistic) {
				// Further check is required as sol may convert nil to nullptr.
				statistic = maybeStatistic.value();
			}
			if (statistic == nullptr) {
				std::stringstream ss;
				ss << "tes3.setStatistic: No statistic named \"" << statisticName.value() << "\" could be found.";
				throw std::invalid_argument(ss.str());
			}
		}

		// This case shouldn't be hit.
		if (statistic == nullptr) {
			throw std::invalid_argument("tes3.setStatistic: No statistic resolved.");
		}

		// Retype our variables to something more friendly, and get additional params.
		sol::optional<bool> limit = params["limit"];

		sol::optional<float> current = params["current"];
		sol::optional<float> base = params["base"];
		sol::optional<float> value = params["value"];

		// Edit both.
		if (value) {
			statistic->setBaseAndCurrent(value.value());
		}
		// If we're given a current value, modify it.
		else if (current) {
			statistic->setCurrentCapped(current.value(), limit.value_or(false));
		}
		// If we're given a base value, modify it.
		else if (base) {
			statistic->setBase(base.value());
		}
		else {
			throw std::invalid_argument("tes3.setStatistic: No edit mode provided, missing parameter 'current' or 'base' or 'value'.");
		}

		// Update any derived statistics.
		mobile->updateDerivedStatistics(statistic);

		// Ensure the reference is flagged as modified.
		if (mobile->reference) {
			mobile->reference->setObjectModified(true);
		}

		// If this was on the player update any associated GUI widgets.
		if (mobile->actorType == TES3::MobileActorType::Player) {
			// Manually update health/magicka/fatigue UI elements.
			if (statistic == &mobile->health) {
				TES3::UI::updateHealthFillBar(statistic->current, statistic->base);
			}
			else if (statistic == &mobile->magicka) {
				TES3::UI::updateMagickaFillBar(statistic->current, statistic->base);
			}
			else if (statistic == &mobile->fatigue) {
				TES3::UI::updateFatigueFillBar(statistic->current, statistic->base);
			}
			else if (statistic == &mobile->encumbrance) {
				TES3::UI::updateEncumbranceBar();
			}
			else {
				// Check to see if an attribute was edited.
				for (size_t i = TES3::Attribute::FirstAttribute; i <= TES3::Attribute::LastAttribute; ++i) {
					if (statistic == &mobile->attributes[i]) {
						TES3::UI::updateEncumbranceBar();
						TES3::UI::updatePlayerAttribute(statistic->getCurrent(), i);
						break;
					}
				}
			}

			TES3::UI::updateStatsPane();
		}
	}

	void modStatistic(sol::table params) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Figure out our mobile object, in case someone gives us a reference instead.
		sol::userdata maybeMobile = params["reference"];
		if (maybeMobile.is<TES3::Reference>()) {
			maybeMobile = maybeMobile["mobile"];
		}

		// Make sure our object is of the right type.
		if (!maybeMobile.is<TES3::MobileActor>()) {
			throw std::invalid_argument("tes3.modStatistic: Could not resolve parameter 'reference'.");
		}

		// Try to get our statistic.
		TES3::MobileActor* mobile = maybeMobile.as<TES3::MobileActor*>();
		TES3::Statistic* statistic = nullptr;
		sol::optional<const char*> statisticName = params["name"];
		sol::optional<int> statisticSkill = params["skill"];
		sol::optional<int> statisticAttribute = params["attribute"];
		if (statisticSkill) {
			if (mobile->actorType == TES3::MobileActorType::Creature) {
				if (statisticSkill.value() >= TES3::CreatureSkillID::FirstSkill && statisticSkill.value() <= TES3::CreatureSkillID::LastSkill) {
					statistic = &static_cast<TES3::MobileCreature*>(mobile)->skills[statisticSkill.value()];
				}
				else {
					std::stringstream ss;
					ss << "tes3.modStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for creature.";
					throw std::invalid_argument(ss.str());
				}
			}
			else {
				if (statisticSkill.value() >= TES3::SkillID::FirstSkill && statisticSkill.value() <= TES3::SkillID::LastSkill) {
					statistic = &static_cast<TES3::MobileNPC*>(mobile)->skills[statisticSkill.value()];
				}
				else {
					std::stringstream ss;
					ss << "tes3.modStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for NPC.";
					throw std::invalid_argument(ss.str());
				}
			}
		}
		else if (statisticAttribute) {
			if (statisticAttribute.value() >= TES3::Attribute::FirstAttribute && statisticAttribute.value() <= TES3::Attribute::LastAttribute) {
				statistic = &mobile->attributes[statisticAttribute.value()];
			}
			else {
				std::stringstream ss;
				ss << "tes3.modStatistic: Invalid attribute index " << std::dec << statisticSkill.value() << ".";
				throw std::invalid_argument(ss.str());
			}
		}
		else if (statisticName) {
			sol::optional<TES3::Statistic*> maybeStatistic = maybeMobile[statisticName.value()];
			if (maybeStatistic) {
				// Further check is required as sol may convert nil to nullptr.
				statistic = maybeStatistic.value();
			}
			if (statistic == nullptr) {
				std::stringstream ss;
				ss << "tes3.modStatistic: No statistic named \"" << statisticName.value() << "\" could be found.";
				throw std::invalid_argument(ss.str());
			}
		}

		// This case shouldn't be hit.
		if (statistic == nullptr) {
			throw std::invalid_argument("tes3.modStatistic: No statistic resolved.");
		}

		// Retype our variables to something more friendly, and get additional params.
		sol::optional<bool> limit = params["limit"];
		sol::optional<bool> limitToBase = params["limitToBase"];

		sol::optional<float> current = params["current"];
		sol::optional<float> base = params["base"];
		sol::optional<float> value = params["value"];

		// Edit both.
		if (value) {
			statistic->modBaseCapped(value.value(), limit.value_or(false), limit.value_or(false));
			statistic->modCurrentCapped(value.value(), limit.value_or(false), limitToBase.value_or(false), limit.value_or(false));
		}
		// If we're given a current value, modify it.
		else if (current) {
			statistic->modCurrentCapped(current.value(), limit.value_or(false), limitToBase.value_or(false), limit.value_or(false));
		}
		// If we're given a base value, modify it.
		else if (base) {
			statistic->modBaseCapped(base.value(), limit.value_or(false), limit.value_or(false));
		}
		else {
			throw std::invalid_argument("tes3.modStatistic: No edit mode provided, missing parameter 'current' or 'base' or 'value'.");
			return;
		}

		// Update any derived statistics.
		mobile->updateDerivedStatistics(statistic);

		// Ensure the reference is flagged as modified.
		if (mobile->reference) {
			mobile->reference->setObjectModified(true);
		}

		// If this was on the player update any associated GUI widgets.
		if (mobile->actorType == TES3::MobileActorType::Player) {
			// Manually update health/magicka/fatigue UI elements.
			if (statistic == &mobile->health) {
				TES3::UI::updateHealthFillBar(statistic->current, statistic->base);
			}
			else if (statistic == &mobile->magicka) {
				TES3::UI::updateMagickaFillBar(statistic->current, statistic->base);
			}
			else if (statistic == &mobile->fatigue) {
				TES3::UI::updateFatigueFillBar(statistic->current, statistic->base);
			}
			else if (statistic == &mobile->encumbrance) {
				TES3::UI::updateEncumbranceBar();
			}
			else {
				// Check to see if an attribute was edited.
				for (size_t i = TES3::Attribute::FirstAttribute; i <= TES3::Attribute::LastAttribute; ++i) {
					if (statistic == &mobile->attributes[i]) {
						TES3::UI::updateEncumbranceBar();
						TES3::UI::updatePlayerAttribute(statistic->getCurrent(), i);
						break;
					}
				}
			}

			TES3::UI::updateStatsPane();
		}
	}

	void modDisposition(sol::table params) {
		auto reference = getOptionalParamExecutionReference(params);
		auto mobile = reference->getAttachedMobileNPC();
		sol::optional<int> value = params["value"];
		bool temporary = getOptionalParam(params, "temporary", false);
		bool inDialogue = false;

		if (!value) {
			throw std::invalid_argument("tes3.modDisposition: 'value' is required.");
		}

		// Check if the player is in dialogue with the target.
		if (mobile) {
			auto menuDialog = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3442));
			if (menuDialog && TES3::UI::getServiceActor() == mobile) {
				inDialogue = true;
			}
		}

		if (temporary) {
			// Only make temporary changes in the dialogue menu. 
			if (inDialogue) {
				// Modify the NPC disposition, with clamping of effective disposition.
				reference->baseObject->modDisposition(value.value());

				// Modify the temporary disposition property and update UI.
				const auto TES3_ui_temporaryDispositionMod = reinterpret_cast<int(__cdecl*)(int)>(0x5C08E0);
				TES3_ui_temporaryDispositionMod(value.value());
			}
		}
		else {
			// Modify the NPC disposition, with clamping of effective disposition.
			reference->baseObject->modDisposition(value.value());

			if (inDialogue) {
				// Modify the permanent disposition property and update UI.
				const auto TES3_ui_permanentDispositionMod = reinterpret_cast<int(__cdecl*)(int)>(0x5C0860);
				TES3_ui_permanentDispositionMod(value.value());
			}
		}
	}

	bool getLegacyScriptRunning(sol::table params) {
		auto script = getOptionalParamScript(params, "script");
		if (script == nullptr) {
			return false;
		}

		return TES3::WorldController::get()->isGlobalScriptRunning(script);
	}

	bool runLegacyScript(sol::table params) {
		TES3::Script* script = getOptionalParamScript(params, "script");
		if (script == nullptr) {
			script = TES3::WorldController::get()->scriptCompileAndRun;
		}

		TES3::ScriptCompiler* compiler = TES3::WorldController::get()->menuController->scriptCompiler;
		int source = getOptionalParam<int>(params, "source", TES3::CompilerSource::Default);
		if (source < TES3::CompilerSource::FirstSource || source > TES3::CompilerSource::LastSource) {
			return false;
		}

		const char* command = getOptionalParam<const char*>(params, "command", nullptr);
		if (command == nullptr) {
			return false;
		}

		TES3::ScriptVariables* variables = getOptionalParam<TES3::ScriptVariables*>(params, "variables", nullptr);

		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference && variables == nullptr) {
			variables = reference->getScriptVariables();
		}

		TES3::Dialogue* dialogue = getOptionalParamDialogue(params, "dialogue");
		TES3::DialogueInfo* dialogueInfo = getOptionalParam< TES3::DialogueInfo*>(params, "info", nullptr);
		if (dialogue == nullptr || dialogueInfo == nullptr) {
			dialogue = nullptr;
			dialogueInfo = nullptr;
			source = TES3::CompilerSource::Default;
		}
		else {
			source = TES3::CompilerSource::Dialogue;
		}

		TES3::DataHandler::suppressThreadLoad = true;
		script->doCommand(compiler, command, source, reference, variables, dialogueInfo, dialogue);
		TES3::DataHandler::suppressThreadLoad = false;

		return true;
	}

	void stopLegacyScript(sol::table params) {
		const auto script = getOptionalParamScript(params, "script");
		if (script == nullptr) {
			throw std::runtime_error("Invalid 'script' parameter provided.");
		}

		TES3::WorldController::get()->stopGlobalScript(script);
	}

	bool force1stPerson() {
		auto worldController = TES3::WorldController::get();
		if (worldController == nullptr) {
			throw std::runtime_error("Function called before world controller is initialized.");
		}

		auto macp = worldController->getMobilePlayer();
		if (macp == nullptr) {
			throw std::runtime_error("Function called while mobile player is unavailable.");
		}

		auto animController = macp->animationController.asPlayer;
		if (animController == nullptr) {
			throw std::runtime_error("Function called while mobile player animation controller is unavailable.");
		}

		return animController->force1stPerson();
	}

	bool force3rdPerson() {
		auto worldController = TES3::WorldController::get();
		if (worldController == nullptr) {
			throw std::runtime_error("Function called before world controller is initialized.");
		}

		auto macp = worldController->getMobilePlayer();
		if (macp == nullptr) {
			throw std::runtime_error("Function called while mobile player is unavailable.");
		}

		auto animController = macp->animationController.asPlayer;
		if (animController == nullptr) {
			throw std::runtime_error("Function called while mobile player animation controller is unavailable.");
		}

		return animController->force3rdPerson();
	}

	sol::object getActiveCells() {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			return sol::nil;
		}

		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		sol::table result = state.create_table();

		if (dataHandler->currentInteriorCell) {
			result[1] = dataHandler->currentInteriorCell;
		}
		else {
			int exteriorCount = 0;
			for (size_t i = 0; i < 9; ++i) {
				auto cellDataPointer = dataHandler->exteriorCellData[i];
				if (cellDataPointer && cellDataPointer->loadingFlags >= 1) {
					exteriorCount++;
					result[exteriorCount] = cellDataPointer->cell;
				}
			}
		}

		return result;
	}

	bool positionCell(sol::table params) {
		auto worldController = TES3::WorldController::get();
		auto macp = worldController->getMobilePlayer();
		auto playerRef = macp->reference;

		// Get the target that we're working with.
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			reference = playerRef;
		}

		// Get the position.
		sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
		if (!position) {
			return false;
		}

		// Get the orientation.
		sol::optional<TES3::Vector3> orientation = getOptionalParamVector3(params, "orientation");
		const auto userProvidedOrientation = orientation.has_value();
		if (!userProvidedOrientation) {
			orientation = reference->orientation;
		}

		// Get the cell.
		TES3::Cell* cell = getOptionalParamCell(params, "cell");
		if (cell == nullptr) {
			// Try to find an exterior cell from the position.
			int gridX = TES3::Cell::toGridCoord(position.value().x);
			int gridY = TES3::Cell::toGridCoord(position.value().y);
			cell = TES3::DataHandler::get()->nonDynamicData->getCellByGrid(gridX, gridY);

			if (cell == nullptr) {
				return false;
			}
		}

		// Are we doing a simple reposition?
		if (cell == reference->getCell() && !getOptionalParam<bool>(params, "forceCellChange", false)) {
			reference->setPosition(&position.value());
			reference->setOrientation(&orientation.value());
			return true;
		}

		// Are we dealing with the player? If so, use the special functions.
		if (reference == playerRef) {
			sol::optional<bool> suppressFaderOpt = params["suppressFader"];
			bool suppressFader = suppressFaderOpt.value_or(false);
			bool faderInitialState = TES3::DataHandler::get()->useCellTransitionFader;

			if (suppressFader) {
				TES3::DataHandler::get()->useCellTransitionFader = false;
			}

			bool suppressThreadLoad = true;
			std::swap(TES3::DataHandler::suppressThreadLoad, suppressThreadLoad);

			sol::optional<bool> teleportCompanions = params["teleportCompanions"];
			if (teleportCompanions.value_or(true) && macp->listFriendlyActors.size() > 0) {
				const auto TES3_cellChangeWithCompanions = reinterpret_cast<void(__cdecl*)(TES3::Vector3, TES3::Vector3, TES3::Cell*)>(0x45C9B0);
				TES3_cellChangeWithCompanions(position.value(), orientation.value(), cell);
			}
			else {
				const auto TES3_cellChange = reinterpret_cast<void(__cdecl*)(TES3::Vector3, TES3::Vector3, TES3::Cell*, int)>(0x45CEF0);
				sol::optional<bool> flag = params["flag"];
				TES3_cellChange(position.value(), orientation.value(), cell, flag.value_or(true));
			}

			std::swap(TES3::DataHandler::suppressThreadLoad, suppressThreadLoad);

			if (suppressFader) {
				TES3::DataHandler::get()->useCellTransitionFader = faderInitialState;
			}
		}
		else {
			bool suppressThreadLoad = true;
			std::swap(TES3::DataHandler::suppressThreadLoad, suppressThreadLoad);

			if (userProvidedOrientation) {
				reference->relocate(cell, &position.value(), orientation.value().z * (180.0f / math::M_PI));
			}
			else {
				reference->relocateNoRotation(cell, &position.value());
			}
			std::swap(TES3::DataHandler::suppressThreadLoad, suppressThreadLoad);

			// Script item data needs to be instanced if the reference is now active but has not been seen before.
			reference->ensureScriptDataIsInstanced();
		}

		// Ensure the reference and cell is flagged as modified.
		reference->setObjectModified(true);
		cell->setObjectModified(true);

		return true;
	}

	static const auto isRussianExe = *reinterpret_cast<DWORD*>(0x40013C) == 0x726F4C5B;
	static const auto isPolishExe = *reinterpret_cast<DWORD*>(0x40008D) == 0x402A7564;

	int getLanguageCode() {
		if (isRussianExe) {
			return 3;
		}
		else if (isPolishExe) {
			return 4;
		}
		return reinterpret_cast<int(__stdcall*)()>(0x4678F0)();
	}

	TES3::Vector3 getLastExteriorPosition() {
		return TES3::DataHandler::get()->getLastExteriorPosition();
	}

	sol::optional<TES3::Vector3> getClosestExteriorPosition(sol::optional<sol::table> params) {
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			// Default to player.
			auto mobilePlayer = TES3::WorldController::get()->getMobilePlayer();
			reference = mobilePlayer->reference;
		}

		auto startCell = reference->getCell();
		if (!startCell->getIsInterior()) {
			return reference->position;
		}

		std::vector<TES3::Cell*> unvisitedCells;
		std::unordered_set<TES3::Cell*> knownCells;
		unvisitedCells.push_back(startCell);
		knownCells.insert(startCell);

		// Breadth-first search cells connected by load doors until an exterior is reached.
		while (unvisitedCells.size() > 0) {
			auto cell = unvisitedCells.back();
			unvisitedCells.pop_back();

			for (auto refr : cell->persistentRefs) {
				if (refr->baseObject->objectType == TES3::ObjectType::Door) {
					auto destination = refr->getAttachedTravelDestination();
					if (destination) {
						auto destinationCell = destination->cell;

						if (destinationCell->getIsInterior()) {
							// Queue up new unseen interior cells.
							if (knownCells.count(destinationCell) == 0) {
								knownCells.insert(destinationCell);
								unvisitedCells.push_back(destinationCell);
							}
						}
						else {
							// For an exterior destination, return the door marker position.
							return destination->destination->position;
						}
					}
				}
			}
		}

		return {};
	}

	sol::optional<std::string> getLanguage() {
		switch (getLanguageCode()) {
		case 0:
			return { "eng" };
		case 1:
			return { "fra" };
		case 2:
			return { "deu" };
		case 3:
			return { "rus" };
		case 4:
			return { "pol" };
		}

		return sol::optional<std::string>();
	}

	bool addSoulGem(sol::table params) {
		TES3::Misc* item = getOptionalParamObject<TES3::Misc>(params, "item");
		if (item == nullptr) {
			return false;
		}

		auto data = mwse::tes3::addCustomSoulGem(item);
		if (data == nullptr) {
			return false;
		}

		return true;
	}

	const auto TES3_UI_updatSpellsList = reinterpret_cast<void(__cdecl*)()>(0x5E3D10);
	const auto TES3_UI_updatEnchantmentsList = reinterpret_cast<void(__cdecl*)()>(0x5E3070);
	void updateMagicGUI_internal(TES3::Reference* reference, bool updateSpells = true, bool updateEnchantments = true) {
		auto worldController = TES3::WorldController::get();
		auto macp = worldController->getMobilePlayer();

		// Player-specific handling.
		if (macp && reference == macp->reference) {
			// Update MenuMagic
			auto menuMagic = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D431E));
			if (menuMagic) {
				if (updateSpells) {
					TES3_UI_updatSpellsList();
				}

				if (updateEnchantments) {
					TES3_UI_updatEnchantmentsList();
				}
			}
		}
	}

	bool addSpell(sol::table params) {
		// Get some complex inputs...
		TES3::Reference* reference = nullptr;
		TES3::BaseObject* object = nullptr;
		TES3::MobileActor* mobile = nullptr;
		if (!getOptionalComplexObjectParams(params, reference, object, mobile, "reference", "mobile", "actor")) {
			throw std::invalid_argument("Could not determine target. Provide a valid reference, mobile, or object.");
		}

		// Make sure we were given an actor that can use spells.
		if (object->objectType != TES3::ObjectType::NPC && object->objectType != TES3::ObjectType::Creature) {
			throw std::invalid_argument("Input target was not a valid spell-supporting actor.");
		}

		// Get a spell to add.
		auto spell = getOptionalParamSpell(params, "spell");
		if (spell == nullptr) {
			throw std::invalid_argument("Invalid 'spell' parameter provided.");
		}

		// Get the spell list.
		auto actor = static_cast<TES3::Actor*>(object);
		auto spellList = actor->getSpellList();

		// Do we already know this spell?
		if (spellList->contains(spell)) {
			return false;
		}

		// Do we already know if because of our race?
		auto race = actor->getRace();
		if (race && race->abilities->contains(spell)) {
			return false;
		}

		// How about our birthsign?
		auto mobilePlayer = TES3::WorldController::get()->getMobilePlayer();
		if (mobilePlayer && mobilePlayer == mobile) {
			if (mobilePlayer->birthsign && mobilePlayer->birthsign->spellList.contains(spell)) {
				return false;
			}
		}

		// We are free to add the spell.
		if (!spellList->add(spell)) {
			return false;
		}

		// Do we need to activate the effects?
		auto instance = nullptr;
		if (mobile && mobile->isActive() && !spell->isActiveCast()) {
			auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;

			TES3::MagicSourceCombo sourceCombo = spell;
			auto serial = magicInstanceController->activateSpell(reference, nullptr, &sourceCombo);
			if (serial != 0) {
				auto instance = magicInstanceController->getInstanceFromSerial(serial);
				if (instance) {
					// Force bypass resistances.
					instance->bypassResistances = getOptionalParam(params, "bypassResistances", true);
					// Process twice so that it becomes active immediately. State transitions are pre-cast -> casting -> working.
					// This makes the effects immediately visible in menu mode.
					// It also prevents a bad state where tes3.removeSpell only removes the spell but not the effect,
					// because it's not active on the target yet.
					instance->process(0);
					instance->process(0);
				}
			}
		}

		// Update GUI elements if necessary.
		if (spell->isActiveCast() && getOptionalParam(params, "updateGUI", true)) {
			updateMagicGUI_internal(reference, true, false);
		}

		// Update modified flags.
		object->getBaseObject()->setObjectModified(true);
		if (reference) {
			reference->setObjectModified(true);
		}

		return true;
	}

	const auto TES3_UI_removeSpellFromGUIList = reinterpret_cast<void(__cdecl*)(TES3::Spell*)>(0x5E3BD0);
	const auto TES3_UI_MenuMagic_refreshAll = reinterpret_cast<void(__cdecl*)(TES3::Spell*)>(0x5E3D10);
	const auto TES3_UI_MenuMagic_deselectMagic = reinterpret_cast<void(__cdecl*)()>(0x5E4500);

	bool removeSpell(sol::table params) {
		// Get some complex inputs...
		TES3::Reference* reference = nullptr;
		TES3::BaseObject* object = nullptr;
		TES3::MobileActor* mobile = nullptr;
		if (!getOptionalComplexObjectParams(params, reference, object, mobile, "reference", "mobile", "actor")) {
			throw std::invalid_argument("Could not determine target. Provide a valid reference, mobile, or object.");
		}

		// Make sure we were given an actor that can use spells.
		if (object->objectType != TES3::ObjectType::NPC && object->objectType != TES3::ObjectType::Creature) {
			throw std::invalid_argument("Input target was not a valid spell-supporting actor.");
		}

		// Get a spell to remove.
		auto spell = getOptionalParamSpell(params, "spell");
		if (spell == nullptr) {
			throw std::invalid_argument("Invalid 'spell' parameter provided.");
		}

		// Get the spell list.
		auto actor = static_cast<TES3::Actor*>(object);
		auto spellList = actor->getSpellList();

		// Remove the spell.
		if (!spellList->remove(spell)) {
			return false;
		}

		// End any active effects for the spell.
		if (mobile && !spell->isActiveCast()) {
			std::unordered_set<unsigned int> instancesToRetire;

			for (const auto& effect : mobile->activeMagicEffects) {
				if (instancesToRetire.find(effect.magicInstanceSerial) != instancesToRetire.end()) {
					continue;
				}

				auto instance = effect.getInstance();
				if (instance) {
					auto source = &instance->sourceCombo;
					if (source->sourceType == TES3::MagicSourceType::Spell && source->source.asSpell == spell) {
						instancesToRetire.insert(effect.magicInstanceSerial);
					}
				}
			}

			if (!instancesToRetire.empty()) {
				auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
				for (const auto serial : instancesToRetire) {
					auto instance = magicInstanceController->getInstanceFromSerial(serial);
					if (instance) {
						instance->retire(reference);
					}
				}
			}
		}

		// Update GUI elements if necessary.
		auto mobilePlayer = TES3::WorldController::get()->getMobilePlayer();
		if (mobilePlayer && mobile == mobilePlayer && getOptionalParam(params, "updateGUI", true)) {
			if (spell->isActiveCast()) {
				// Deselect if currently selected.
				if (mobilePlayer->currentSpell.source.asSpell == spell) {
					TES3_UI_MenuMagic_deselectMagic();
				}

				// Magic menu spell list update.
				if (spell->castType == TES3::SpellCastType::Spell) {
					TES3_UI_removeSpellFromGUIList(spell);
				}
				else {
					// Full list refresh is required for powers UI to update.
					TES3_UI_MenuMagic_refreshAll(spell);
				}
			}
			else {
				// Active effects icon bar.
				TES3::MagicInstanceController::updateActiveMagicEffectIcons();
			}
		}

		// Update modified flags.
		object->getBaseObject()->setObjectModified(true);
		if (reference) {
			reference->setObjectModified(true);
		}

		return true;
	}

	bool hasSpell(sol::table params) {
		// Get some complex inputs...
		TES3::Reference* reference = nullptr;
		TES3::BaseObject* object = nullptr;
		TES3::MobileActor* mobile = nullptr;
		if (!getOptionalComplexObjectParams(params, reference, object, mobile, "reference", "mobile", "actor")) {
			throw std::invalid_argument("Could not determine target. Provide a valid reference, mobile, or object.");
		}

		// Make sure we were given an actor that can use spells.
		if (object->objectType != TES3::ObjectType::NPC && object->objectType != TES3::ObjectType::Creature) {
			throw std::invalid_argument("Input target was not a valid spell-supporting actor.");
		}

		// Get a spell to check.
		auto spell = getOptionalParamSpell(params, "spell");
		if (spell == nullptr) {
			throw std::invalid_argument("Invalid 'spell' parameter provided.");
		}

		// Check the racial list.
		auto actor = static_cast<TES3::Actor*>(object);
		auto race = actor->getRace();
		if (race && race->abilities->contains(spell)) {
			return true;
		}

		// How about our birthsign?
		auto mobilePlayer = TES3::WorldController::get()->getMobilePlayer();
		if (mobilePlayer && mobilePlayer == mobile) {
			if (mobilePlayer->birthsign && mobilePlayer->birthsign->spellList.contains(spell)) {
				return true;
			}
		}

		// Now for the main spell list...
		auto spellList = actor->getSpellList();
		if (spellList->contains(spell)) {
			return true;
		}

		return false;
	}

	void addArmorSlot(sol::this_state ts, sol::table params) {
		sol::optional<int> slot = params["slot"];
		if (!slot || (slot.value() >= TES3::ArmorSlot::First && slot.value() <= TES3::ArmorSlot::Last)) {
			throw std::exception("tes3.addArmorSlot: Invalid slot. A vanilla slot cannot be defined.");
		}

		sol::optional<std::string> name = params["name"];
		if (!name || name.value().empty()) {
			throw std::exception("tes3.addArmorSlot: No name provided for slot.");
		}

		sol::optional<float> weight = params["weight"];
		sol::optional<float> armorScalar = params["scalar"];

		const auto& existing = mwse::tes3::getArmorSlotData(slot.value());
		if (existing) {
			// Don't error if we're adding the same slot with the same name.
			if (existing->name == name) {
				// Do override the weight/armor scalar though in case of an updated mod.
				existing->weight = weight.value_or(existing->weight);
				existing->armorScalar = armorScalar.value_or(existing->armorScalar);
				return;
			}
			throw std::exception("tes3.addArmorSlot: Invalid slot. The slot already exists with a different name.");
		}

		auto slotData = std::make_shared<TES3::ArmorSlotData>();
		slotData->slot = slot.value();
		slotData->name = name.value();
		slotData->weight = weight.value_or(0.0f);
		slotData->armorScalar = armorScalar.value_or(0.1f);

		mwse::tes3::setArmorSlotData(slotData);

		// Add the slot to the tes3.armorSlot table.
		sol::state_view state = ts;
		sol::optional<std::string> key = params["key"];
		if (key) {
			state["tes3"]["armorSlot"][key] = slot.value();
		}
		else {
			state["tes3"]["armorSlot"][name] = slot.value();
		}
	}

	void addClothingSlot(sol::this_state ts, sol::table params) {
		sol::optional<int> slot = params["slot"];
		if (!slot || (slot.value() >= TES3::ClothingSlot::First && slot.value() <= TES3::ClothingSlot::Last)) {
			throw std::exception("tes3.addClothingSlot: Invalid slot. A vanilla slot cannot be defined.");
		}

		if (slot.value() < 0 || slot.value() > UINT8_MAX) {
			throw std::exception("tes3.addClothingSlot: Invalid slot. Value must be between 10 and 255.");
		}

		sol::optional<std::string> name = params["name"];
		if (!name || name.value().empty()) {
			throw std::exception("tes3.addClothingSlot: No name provided for slot.");
		}

		const auto& existing = mwse::tes3::getClothingSlotData(slot.value());
		if (existing) {
			// Don't error if we're adding the same slot with the same name.
			if (existing->name == name) {
				return;
			}
			throw std::exception("tes3.addClothingSlot: Invalid slot. The slot already exists with a different name.");
		}

		auto slotData = std::make_shared<TES3::ClothingSlotData>();
		slotData->slot = slot.value();
		slotData->name = name.value();

		mwse::tes3::setClothingSlotData(slotData);

		// Add the slot to the tes3.clothingSlot table.
		sol::state_view state = ts;
		sol::optional<std::string> key = params["key"];
		if (key) {
			state["tes3"]["clothingSlot"][key] = slot.value();
		}
		else {
			state["tes3"]["clothingSlot"][name] = slot.value();
		}
	}

	// WARNING: This function doesn't persist through saves correctly and shouldn't be used.
	TES3::Cell* createCell(sol::table params) {
		auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;

		TES3::Cell* cell = nullptr;

		sol::optional<int> gridX = params["gridX"];
		sol::optional<int> gridY = params["gridY"];
		if (gridX && gridY) {
			auto existingCell = nonDynamicData->getCellByGrid(gridX.value(), gridY.value());
			if (existingCell) {
				mwse::log::getLog() << "Could not create cell at coordinates <" << gridX.value() << ", " << gridY.value() << ">. Cell already exists at that location." << std::endl;
				return nullptr;
			}

			cell = TES3::Cell::create();
			cell->setIsInterior(false);
			cell->setGridX(gridX.value());
			cell->setGridY(gridY.value());
			cell->setName("");
		}
		else {
			sol::optional<const char*> name = params["name"];
			if (!name) {
				mwse::log::getLog() << "Could not create cell. Interior cells must have a name." << std::endl;
				return nullptr;
			}

			auto existingCell = nonDynamicData->getCellByName(name.value());
			if (existingCell) {
				mwse::log::getLog() << "Could not create cell \"" << name.value() << "\". Cell already exists with the given name." << std::endl;
				return nullptr;
			}

			cell = TES3::Cell::create();
			cell->setIsInterior(true);
			cell->setName(name.value());
		}

		cell->setObjectModified(true);

		nonDynamicData->cells->push_back(cell);

		return cell;
	}

	TES3::Reference* createReference(sol::table params) {
		auto dataHandler = TES3::DataHandler::get();

		// Get the object we are going to create a reference for.
		auto object = getOptionalParamObject<TES3::PhysicalObject>(params, "object");
		if (object == nullptr) {
			throw std::invalid_argument("Invalid 'object' parameter provided.");
		}

		// Make sure we have a base object.
		object = static_cast<TES3::PhysicalObject*>(object->getBaseObject());

		// Get the position.
		auto maybePosition = getOptionalParamVector3(params, "position");
		if (!maybePosition) {
			throw std::invalid_argument("Invalid 'position' parameter provided.");
		}

		// Get the orientation.
		TES3::Vector3 orientation(0.0f, 0.0f, 0.0f);
		auto maybeOrientation = getOptionalParamVector3(params, "orientation");
		if (maybeOrientation) {
			orientation = maybeOrientation.value();
		}

		// Try to resolve the sell, either by what we were given, or a valid cell based on the given position.
		TES3::Cell* cell = getOptionalParamCell(params, "cell");
		if (cell == nullptr || (!cell->getIsInterior() && !cell->isPointInCell(maybePosition.value().x, maybePosition.value().y))) {
			int cellX = TES3::Cell::toGridCoord(maybePosition.value().x);
			int cellY = TES3::Cell::toGridCoord(maybePosition.value().y);
			cell = dataHandler->nonDynamicData->getCellByGrid(cellX, cellY);
		}

		// Make sure we actually got a cell.
		if (cell == nullptr) {
			throw std::invalid_argument("Invalid 'cell' parameter provided. Please provide a cell, or give a valid exterior position.");
		}

		// Create reference.
		TES3::Reference* reference = new TES3::Reference();
		reference->baseObject = object;

		// Add it to the cell lists/data handler.
		bool cellWasCreated = false;
		dataHandler->nonDynamicData->createReference(object, &maybePosition.value(), &orientation, cellWasCreated, reference, cell);
		reference->ensureScriptDataIsInstanced();
		cell->insertReference(reference);

		// Scale as needed.
		const auto scale = getOptionalParam<float>(params, "scale");
		if (scale) {
			reference->setScale(scale.value());
		}

		// Did we just make an actor? If so we need to add it to the mob manager.
		if (object->objectType == TES3::ObjectType::Creature || object->objectType == TES3::ObjectType::NPC) {
			TES3::WorldController::get()->mobManager->addMob(reference);
			auto mact = reference->getAttachedMobileActor();
			if (mact && mact->isActor()) {
				mact->enterLeaveSimulation(true);
			}
		}
		// Activators, containers, and statics need collision.
		else if (object->objectType == TES3::ObjectType::Activator || object->objectType == TES3::ObjectType::Container || object->objectType == TES3::ObjectType::Static) {
			dataHandler->updateCollisionGroupsForActiveCells();
		}
		// Lights need to be configured.
		else if (object->objectType == TES3::ObjectType::Light) {
			dataHandler->setDynamicLightingForReference(reference);

			// Non-carryable lights also need collision.
			if (!static_cast<TES3::Light*>(object)->getCanCarry()) {
				dataHandler->updateCollisionGroupsForActiveCells();
			}
		}

		// Ensure the reference receives scene lighting.
		dataHandler->updateLightingForReference(reference);

		// Make sure everything is set as modified.
		reference->setObjectModified(true);
		cell->setObjectModified(true);

		return reference;
	}

	TES3::BaseObject* createObject(sol::table params) {
		const char* id = getOptionalParam<const char*>(params, "id", nullptr);
		if (id && strlen(id) == 0) {
			throw std::invalid_argument("Invalid 'id' parameter provided.");
		}

		auto objectType = getOptionalParam(params, "objectType", TES3::ObjectType::Invalid);
		if (objectType == TES3::ObjectType::Invalid) {
			throw std::invalid_argument("Invalid 'objectType' parameter provided.");
		}
		auto getIfExists = getOptionalParam(params, "getIfExists", true);
		return makeObjectCreator(objectType)->create(params, getIfExists);
	}

	bool setDestination(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		if (reference->baseObject->objectType != TES3::ObjectType::Door) {
			throw std::invalid_argument("Provided reference is not a door.");
		}

		// Get the position.
		sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
		if (!position) {
			throw std::invalid_argument("Invalid position parameter provided.");
		}

		// Get the orientation.
		sol::optional<TES3::Vector3> orientation = getOptionalParamVector3(params, "orientation");
		if (!orientation) {
			throw std::invalid_argument("Invalid orientation parameter provided.");
		}

		// Get the cell.
		TES3::Cell* cell = getOptionalParamCell(params, "cell");
		if (cell == nullptr) {
			cell = TES3::DataHandler::get()->nonDynamicData->getCellByPosition(position->x, position->y);
			if (cell == nullptr) {
				throw std::invalid_argument("Invalid position parameter provided. It does not resolve to a valid exterior cell.");
			}
		}

		reference->setTravelDestination(&position.value(), &orientation.value(), cell);
		reference->setObjectModified(true);
		return true;
	}

	bool cast(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		TES3::Spell* spell = getOptionalParamSpell(params, "spell");
		if (spell == nullptr) {
			throw std::invalid_argument("Invalid spell parameter provided.");
		}

		TES3::MobileActor* casterMobile = reference->getAttachedMobileActor();
		TES3::Reference* target = getOptionalParamReference(params, "target");
		if (target == nullptr) {
			// Only allow player to cast without a target.
			if (!(casterMobile && casterMobile->actorType == TES3::MobileActorType::Player)) {
				throw std::invalid_argument("Invalid target parameter provided.");
			}
		}

		bool instant = getOptionalParam<bool>(params, "instant", false);
		bool bypassResistances = getOptionalParam<bool>(params, "bypassResistances", false);
		bool alwaysSucceeds = getOptionalParam<bool>(params, "alwaysSucceeds", true);

		if (casterMobile && !instant) {
			// Let the player cast the spell, if possible.
			if (casterMobile->actorType == TES3::MobileActorType::Player) {
				if (!casterMobile->canAct()) {
					return false;
				}

				// Equip the spell to be casted temporarily since animations depend on it.
				TES3::Object* previousMagicSource = casterMobile->currentSpell.source.asGeneric;
				TES3::EquipmentStack previousEnchantedItem = casterMobile->currentEnchantedItem;
				casterMobile->equipMagic(spell, nullptr, false, false);

				// Activate the spell.
				TES3::MagicSourceCombo sourceCombo(spell);
				TES3::MagicInstanceController* magicInstanceController = TES3::WorldController::get()->magicInstanceController;
				unsigned int serialNumber = magicInstanceController->activateSpell(casterMobile->reference, nullptr, &sourceCombo);
				if (serialNumber) {
					// Set lua parameters on the spell instance.
					TES3::MagicSourceInstance* spellInstance = magicInstanceController->getInstanceFromSerial(serialNumber);
					spellInstance->target = target;
					spellInstance->bypassResistances = bypassResistances;
					if (alwaysSucceeds) {
						spellInstance->overrideCastChance = 100.0f;
					}

					// Play the cast animation.
					TES3::PlayerAnimationController* animationController = casterMobile->animationController.asPlayer;
					animationController->animationData->timing[1] = 0.0;
					animationController->startCastAnimation();
				}

				// Reset the equipped magic.
				if (previousMagicSource) {
					if (previousMagicSource->objectType == TES3::ObjectType::Enchantment) {
						casterMobile->equipMagic(previousEnchantedItem.object, previousEnchantedItem.itemData, false, false);
					}
					else {
						casterMobile->equipMagic(previousMagicSource, nullptr, false, false);
					}
				}
				else {
					casterMobile->unequipMagic();
				}

				return true;
			}
			// Request AI to cast chosen spell.
			else if (casterMobile->isActive() && target != nullptr) {
				casterMobile->setCurrentMagicFromSpell(spell);
				casterMobile->forceSpellCast(target->getAttachedMobileActor());
				return true;
			}
		}
		else {
			// Instant cast from both actors and non-actors.
			TES3::MagicSourceCombo sourceCombo(spell);
			auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
			auto serial = magicInstanceController->activateSpell(reference, nullptr, &sourceCombo);
			auto spellInstance = magicInstanceController->getInstanceFromSerial(serial);

			if (alwaysSucceeds) {
				spellInstance->overrideCastChance = 100.0f;
			}
			spellInstance->target = target;
			spellInstance->bypassResistances = bypassResistances;

			// Trigger spells to progress from pre-cast to targetting state. This state is automatically reset by active AI.
			if (casterMobile) {
				// Force animation state to cause the magic instance to directly proceed to casting.
				// This interrupts other attack actions.
				casterMobile->actionData.animStateAttack = TES3::AttackAnimationState::CastingFollow;
				// Process the magic to avoid a sequencing issue where the mobile's animation controller may cancel the casting
				// if this function is called after the magic controller runs, but before the animation controller runs.
				spellInstance->process(0.0f);
			}

			return true;
		}

		return false;
	}

	static TES3::EquipmentStack tempApplyMagicSourceStack;
	TES3::MagicSourceInstance* applyMagicSource(sol::table params) {
		// Get who we're adding the magic effect to.
		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		// Get the source for the effect.
		auto source = getOptionalParamObject<TES3::Object>(params, "source");

		// Are we given custom effects? If so make a potion.
		bool createCopy = true;
		sol::optional<sol::table> effects = params["effects"];
		if (source == nullptr && effects) {
			const char* name = getOptionalParam<const char*>(params, "name", nullptr);
			if (!name) {
				throw std::invalid_argument("Invalid 'name' parameter provided when creating dynamic effect.");
			}

			// Dynamically create our potion.
			auto dynamicPotion = new TES3::Alchemy();
			dynamicPotion->setName(name);
			dynamicPotion->flags |= 0x2;
			TES3::DataHandler::get()->nonDynamicData->addNewObject(dynamicPotion);

			// Assign effects.
			if (effects) {
				for (int i = 1; i <= 8; ++i) {
					sol::optional<sol::table> effectParams = effects.value()[i];
					if (!effectParams) {
						break;
					}

					dynamicPotion->effects[i - 1] = effectParams.value();

					// Legacy parameter name for rangeType.
					sol::optional<TES3::EffectRange> range = effectParams.value()["range"];
					if (range) {
						dynamicPotion->effects[i - 1].rangeType = range.value();
					}
				}
			}

			// If we don't have an effect, fill in the first one.
			if (dynamicPotion->effects[0].effectID == TES3::EffectID::None) {
				dynamicPotion->effects[0].effectID = TES3::EffectID::WaterBreathing;
			}

			source = dynamicPotion;
			createCopy = false;
		}

		// Did we end up figuring out a source?
		if (source == nullptr) {
			throw std::invalid_argument("Invalid 'source' parameter provided.");
		}

		// Try to make a source for this object.
		TES3::MagicSourceCombo sourceCombo(source);
		if (sourceCombo.sourceType == TES3::MagicSourceType::Invalid) {
			throw std::invalid_argument("Invalid 'source' parameter provided: Not a valid source type.");
		}

		// Do we have a piece of equipment this is coming from?
		auto from = getOptionalParam<TES3::EquipmentStack*>(params, "fromStack");

		// Handle some alchemy patching.
		if (sourceCombo.sourceType == TES3::MagicSourceType::Alchemy) {
			// Do we need to make a copy of this object?
			if (getOptionalParam<bool>(params, "createCopy", createCopy)) {
				auto copy = new TES3::Alchemy();
				copy->copy(source);
				copy->setID("");
				TES3::DataHandler::get()->nonDynamicData->addNewObject(copy);

				source = copy;
				sourceCombo.source.asAlchemy = copy;
			}

			if (!from) {
				tempApplyMagicSourceStack.object = source;
				from = &tempApplyMagicSourceStack;
			}
		}

		// Activate the source on our target.
		auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
		auto serial = magicInstanceController->activateSpell(reference, from.value_or(nullptr), &sourceCombo);
		auto instance = magicInstanceController->getInstanceFromSerial(serial);

		// Check if magic activation succeeded before setting more data.
		if (instance) {
			// Specify target.
			instance->target = getOptionalParamReference(params, "target");

			// Bypass resistances?
			instance->bypassResistances = getOptionalParam<bool>(params, "bypassResistances", false);

			if (sourceCombo.sourceType == TES3::MagicSourceType::Spell) {
				// Force spells to apply all effects to the target immediately. Use caster if no target is specified.
				// This method bypasses cast chance and animations.
				const auto& effects = sourceCombo.source.asSpell->effects;
				auto spellTarget = instance->target ? instance->target : reference;

				for (int i = 0; i < 8; ++i) {
					if (effects[i].effectID != -1) {
						instance->spellHit(spellTarget, i);
					}
				}
				instance->state = TES3::SpellEffectState::Working;
			}
			else if (sourceCombo.sourceType == TES3::MagicSourceType::Enchantment) {
				// Add enchantment source item to recharger.
				auto stack = from.value();
				TES3::WorldController::get()->rechargerAddItem(stack->object, stack->itemData, sourceCombo.source.asEnchantment);
			}
		}

		return instance;
	}

	const auto TES3_MobileActor_applyEquippedConstantEffectEnchs = reinterpret_cast<void(__thiscall*)(TES3::MobileActor*)>(0x52CF40);
	const auto TES3_MobileActor_removeEquippedConstantEffectEnchs = reinterpret_cast<void(__thiscall*)(TES3::MobileActor*)>(0x52D010);
	void applyConstantEffectEquipment(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		TES3::MobileActor* mobile = reference->getAttachedMobileActor();
		bool activate = getOptionalParam<bool>(params, "activate", false);
		bool deactivate = getOptionalParam<bool>(params, "deactivate", false);

		if (activate) {
			TES3_MobileActor_applyEquippedConstantEffectEnchs(mobile);
		}
		else if (deactivate) {
			TES3_MobileActor_removeEquippedConstantEffectEnchs(mobile);
		}
	}

	TES3::MagicSourceInstance* getMagicSourceInstanceBySerial(sol::table params) {
		auto serialNumber = getOptionalParam<unsigned int>(params, "serialNumber", UINT32_MAX);
		if (serialNumber == UINT32_MAX) {
			throw std::invalid_argument("Invalid 'serialNumber' parameter provided.");
		}

		return TES3::WorldController::get()->magicInstanceController->getInstanceFromSerial(serialNumber);
	}

	const auto TES3_UI_showAlchemyMenu = reinterpret_cast<void(__cdecl*)()>(0x599A30);
	void showAlchemyMenu() {
		TES3_UI_showAlchemyMenu();
	}

	const auto TES3_UI_closeAlchemyMenu = reinterpret_cast<char(__cdecl*)()>(0x59B9F0);
	void closeAlchemyMenu() {
		TES3::UI::Element* menuAlchemy = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D225C));
		if (menuAlchemy) {
			TES3_UI_closeAlchemyMenu();
		}
	}

	const auto TES3_UI_showRepairServiceMenu = reinterpret_cast<void(__cdecl*)(TES3::MobileActor*)>(0x615160);
	void showRepairServiceMenu(sol::optional<sol::table> params) {
		TES3::MobileActor* serviceActor = getOptionalParamMobileActor(params, "serviceActor");
		if (!serviceActor) {
			serviceActor = TES3::WorldController::get()->getMobilePlayer();
		}
		TES3_UI_showRepairServiceMenu(serviceActor);
		TES3::UI::enterMenuMode("MenuServiceRepair");
	}

	const auto TES3_UI_closeRepairServiceMenu = reinterpret_cast<char(__cdecl*)()>(0x615520);
	void closeRepairServiceMenu() {
		TES3::UI::Element* menuRepairService = TES3::UI::findMenu("MenuServiceRepair");
		if (menuRepairService) {
			TES3_UI_closeRepairServiceMenu();
		}
	}

	void updateInventoryGUI_internal(TES3::Reference* reference, std::optional<float> containerWeight = {}) {
		auto worldController = TES3::WorldController::get();
		auto macp = worldController->getMobilePlayer();

		// Update inventory menu if necessary.
		if (macp && reference == macp->reference) {
			worldController->inventoryData->clearIcons(2);
			worldController->inventoryData->addInventoryItems(&macp->npcInstance->inventory, 2);
			mwse::tes3::ui::inventoryUpdateIcons();
		}

		// Update contents menu if necessary.
		auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
		if (contentsMenu) {
			TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
			if (reference == contentsReference) {
				float isCompanion = *reinterpret_cast<float*>(0x7D3184);
				if (isCompanion != 0.0f) {
					TES3::UI::updateContentsCompanionElements();
				}

				TES3::UI::updateContentsMenuTiles();

				// Update container weight variable.
				auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
				if (!containerWeight.has_value()) {
					containerWeight = static_cast<TES3::Actor*>(contentsReference->baseObject)->inventory.calculateContainedWeight();
				}
				contentsMenu->setProperty(MenuContents_totalweight, containerWeight.value());
			}
		}
	}

	void updateInventoryGUI(sol::table params) {
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		if (reference) {
			updateInventoryGUI_internal(reference);
		}
	}

	void updateMagicGUI(sol::optional<sol::table> params) {
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		if (reference) {
			updateMagicGUI_internal(reference, getOptionalParam(params, "updateSpells", true), getOptionalParam(params, "updateEnchantments", true));
		}
	}

	std::tuple<int, TES3::Item*, TES3::ItemData*> addItem(sol::table params) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Get the reference we are manipulating.
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		// Get the item we are going to add.
		TES3::Item* item = nullptr;
		TES3::PhysicalObject* itemBase = getOptionalParamObject<TES3::PhysicalObject>(params, "item");
		if (itemBase == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}
		else if (itemBase->objectType == TES3::ObjectType::LeveledItem) {
			item = static_cast<TES3::Item*>(static_cast<TES3::LeveledItem*>(itemBase)->resolve());
			if (!item) {
				return { 0, nullptr, nullptr };
			}
			else if (!item->isItem()) {
				throw std::invalid_argument("Unexpected item case. Report this issue.");
			}
		}
		else if (itemBase->isItem()) {
			item = static_cast<TES3::Item*>(itemBase);
		}
		else {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}

		TES3::ItemData* itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);

		// Make sure we're dealing with actors.
		TES3::Actor* actor = static_cast<TES3::Actor*>(reference->baseObject);
		if (!actor->isActor()) {
			throw std::invalid_argument("The 'reference' reference does not point to an actor.");
		}

		// Enable defining a soul to add.
		TES3::Actor* soul = getOptionalParamObject<TES3::Actor>(params, "soul");
		bool createdItemData = false;
		if (soul) {
			if (!tes3::isSoulGem(item)) {
				throw std::invalid_argument("Cannot add a soul to item, as it is not a soul gem.");
			}

			if (!itemData) {
				itemData = TES3::ItemData::createForObject(item);
				createdItemData = true;
			}

			itemData->soul = soul;
		}

		// Clone the object if needed.
		if (reference->clone()) {
			actor = static_cast<TES3::Actor*>(reference->baseObject);
		}

		// Get how many items we are adding.
		int fulfilledCount = 0;
		int desiredCount = std::max(std::abs(getOptionalParam(params, "count", 1)), 1);
		if (itemData != nullptr) {
			desiredCount = 1;
		}

		if (actor->objectType == TES3::ObjectType::Container && getOptionalParam<bool>(params, "limit", false)) {
			// Prevent placing items into organic containers.
			if (BIT_TEST(actor->actorFlags, TES3::ActorFlagContainer::OrganicBit)) {
				if (createdItemData) {
					delete itemData;
					itemData = nullptr;
				}
				return { 0, nullptr, nullptr };
			}

			// Figure out how many more of the item can fit in the container.
			auto maxCapacity = static_cast<TES3::Container*>(reference->getBaseObject())->capacity;
			auto currentWeight = actor->inventory.calculateContainedWeight();
			fulfilledCount = std::min((int)((maxCapacity - currentWeight) / item->getWeight()), desiredCount);
		}
		else {
			fulfilledCount = desiredCount;
		}

		// No items to add? Great, let's get out of here.
		if (fulfilledCount == 0) {
			if (createdItemData) {
				delete itemData;
				itemData = nullptr;
			}
			return { 0, nullptr, nullptr };
		}

		if (itemData) {
			// Clear the owner, if any.
			itemData->owner = nullptr;

			// Delete the item data if it's fully repaired.
			if (TES3::ItemData::isItemDataStackable(itemData, item)) {
				delete itemData;
				itemData = nullptr;
			}
		}

		// Add the item and return the added count, since we do no inventory checking.
		auto mobile = reference->getAttachedMobileActor();
		if (itemData) {
			actor->inventory.addItem(mobile, item, fulfilledCount, false, &itemData);
		}
		else {
			actor->inventory.addItemWithoutData(mobile, item, fulfilledCount, false);
		}

		// Play the relevant sound.
		auto worldController = TES3::WorldController::get();
		auto playerMobile = worldController->getMobilePlayer();
		if (mobile == playerMobile) {
			if (getOptionalParam<bool>(params, "playSound", true)) {
				worldController->playItemUpDownSound(item, TES3::ItemSoundState::Up);
			}

			if (getOptionalParam<bool>(params, "showMessage", false)) {
				std::string message;
				auto luaFormat = state["string"]["format"];
				auto GMSTs = TES3::DataHandler::get()->nonDynamicData->GMSTs;
				if (fulfilledCount > 1) {
					message = luaFormat(GMSTs[TES3::GMST::sNotifyMessage61]->value.asString, fulfilledCount, item->getName());
				}
				else {
					message = luaFormat(GMSTs[TES3::GMST::sNotifyMessage60]->value.asString, item->getName());
				}
				TES3::UI::showMessageBox(message.c_str());
			}
		}

		// Update equipment if needed.
		// TODO: This can be more efficient by just comparing the newly added item to the equipped item.
		if (reference != playerMobile->reference && item->promptsEquipmentReevaluation() && getOptionalParam<bool>(params, "reevaluateEquipment", true)) {
			actor->reevaluateEquipment();
		}

		// Update body parts for creatures/NPCs that may have items unequipped.
		if (mobile) {
			reference->updateBipedParts();

			if (mobile == playerMobile) {
				playerMobile->firstPersonReference->updateBipedParts();
			}

			mobile->updateOpacity();
		}

		// If the item matches our readied ammo, update the ammo count.
		if (mobile && item->isWeaponOrAmmo() && getOptionalParam<bool>(params, "equipProjectiles", true)) {
			auto asWeapon = static_cast<TES3::Weapon*>(item);
			if (asWeapon->isProjectile() && mobile->readiedAmmo && mobile->readiedAmmo->object == item) {
				mobile->readiedAmmoCount += fulfilledCount;
			}
		}

		// If either of them are the player, we need to update the GUI.
		if (playerMobile && getOptionalParam<bool>(params, "updateGUI", true)) {
			// Update contents menu if necessary.
			std::optional<float> newContainerWeight;
			auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
			if (contentsMenu) {
				TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
				if (reference == contentsReference) {
					TES3::UI::updateContentsMenuTiles();

					// Update container weight variable.
					auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
					auto currentWeight = contentsMenu->getProperty(TES3::UI::PropertyType::Float, MenuContents_totalweight).floatValue;
					newContainerWeight = currentWeight + item->getWeight() * fulfilledCount;
				}
			}
			updateInventoryGUI_internal(reference, newContainerWeight);

			// Do we need to update the magic menu?
			auto enchantment = item->getEnchantment();
			if (enchantment && (enchantment->castType == TES3::EnchantmentCastType::Once || enchantment->castType == TES3::EnchantmentCastType::OnUse)) {
				updateMagicGUI_internal(reference, false, true);
			}
		}

		reference->setObjectModified(true);
		return { fulfilledCount, item, itemData };
	}

	int removeItem(sol::table params) {
		// Get the reference we are manipulating.
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		// Get the item we are going to remove.
		TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}

		TES3::ItemData* itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);

		// Make sure we're dealing with actors.
		TES3::Actor* actor = static_cast<TES3::Actor*>(reference->baseObject);
		if (!actor->isActor()) {
			throw std::invalid_argument("The 'reference' reference does not point to an actor.");
		}

		// Clone the object if needed.
		if (reference->clone()) {
			actor = static_cast<TES3::Actor*>(reference->baseObject);
		}

		// Get how many items we are removing. Force to 1 if we supply an itemData.
		int desiredCount = std::max(std::abs(getOptionalParam(params, "count", 1)), 1);
		if (itemData != nullptr) {
			desiredCount = 1;
		}

		// Make sure that the inventory contains the item.
		TES3::ItemStack* stack = actor->inventory.findItemStack(item);
		if (stack == nullptr) {
			return 0;
		}

		// If we were given an itemData, make sure that it's here.
		if (itemData != nullptr && stack->variables != nullptr && !stack->variables->contains(itemData)) {
			return 0;
		}

		// Limit removal by stack count.
		int fulfilledCount = std::min(desiredCount, stack->count);

		// No items to remove? Great, let's get out of here.
		if (fulfilledCount == 0) {
			return 0;
		}

		// Were we given an ItemData? If so, we only need to remove one item.
		auto mobile = reference->getAttachedMobileActor();
		if (itemData) {
			actor->inventory.removeItemWithData(mobile, item, itemData, 1, false);
			actor->unequipItem(item, true, mobile, false, itemData);
			actor->postUnequipUIRefresh(mobile);
		}
		// No ItemData? We have to go through and remove items over one by one.
		else {
			auto equipStack = actor->getEquippedItem(item);
			int itemsLeftToRemove = fulfilledCount;

			// Remove items without data first. Stack may be deleted after this point.
			int countWithoutVariables = stack->count - (stack->variables ? stack->variables->endIndex : 0);
			if (countWithoutVariables > 0) {
				int amountToRemove = std::min(countWithoutVariables, itemsLeftToRemove);
				actor->inventory.removeItemWithData(mobile, item, nullptr, amountToRemove, false);
				itemsLeftToRemove -= amountToRemove;

				// Update ammunition count and unequip ammunition when exhausted.
				// Located here because ammunition does not generate itemData when equipped.
				if (equipStack && mobile && mobile->readiedAmmo == equipStack) {
					mobile->readiedAmmoCount = std::max(0, int(mobile->readiedAmmoCount) - amountToRemove);
					if (mobile->readiedAmmoCount == 0) {
						actor->unequipItem(item, true, mobile, false, itemData);
						actor->postUnequipUIRefresh(mobile);
					}
				}
			}

			// Then remove items with data, one at a time.
			for (; itemsLeftToRemove > 0; itemsLeftToRemove--) {
				TES3::ItemData* firstAvailableItemData = nullptr;

				if (stack->variables) {
					firstAvailableItemData = stack->variables->at(0);

					// Unequip if itemData matches.
					if (equipStack && firstAvailableItemData == equipStack->itemData) {
						actor->unequipItem(item, true, mobile, false, itemData);
						actor->postUnequipUIRefresh(mobile);
						equipStack = nullptr;
					}
				}

				actor->inventory.removeItemWithData(mobile, item, firstAvailableItemData, 1, false);
			}
		}

		// Play the relevant sound.
		auto worldController = TES3::WorldController::get();
		auto playerMobile = worldController->getMobilePlayer();
		if (mobile == playerMobile && getOptionalParam<bool>(params, "playSound", true)) {
			if (mobile == playerMobile) {
				worldController->playItemUpDownSound(item, TES3::ItemSoundState::Down);
			}
		}

		// Update equipment if needed.
		// TODO: This can be more efficient by just comparing the newly added item to the equipped item.
		if (reference != playerMobile->reference && item->promptsEquipmentReevaluation() && getOptionalParam<bool>(params, "reevaluateEquipment", true)) {
			actor->reevaluateEquipment();
		}

		// If either of them are the player, we need to update the GUI.
		if (playerMobile && getOptionalParam<bool>(params, "updateGUI", true)) {
			// Update contents menu if necessary.
			std::optional<float> newContainerWeight;
			auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
			if (contentsMenu) {
				TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
				if (reference == contentsReference) {
					TES3::UI::updateContentsMenuTiles();
				}

				// Update container weight variable.
				auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
				auto currentWeight = contentsMenu->getProperty(TES3::UI::PropertyType::Float, MenuContents_totalweight).floatValue;
				newContainerWeight = currentWeight - item->getWeight() * fulfilledCount;
			}
			updateInventoryGUI_internal(reference, newContainerWeight);

			// Do we need to update the magic menu?
			auto enchantment = item->getEnchantment();
			if (enchantment && (enchantment->castType == TES3::EnchantmentCastType::Once || enchantment->castType == TES3::EnchantmentCastType::OnUse)) {
				updateMagicGUI_internal(reference, false, true);
			}
		}

		reference->setObjectModified(true);
		return fulfilledCount;
	}

	bool togglePOV() {
		auto worldController = TES3::WorldController::get();
		if (worldController == nullptr) {
			throw std::runtime_error("Function called before world controller is initialized.");
		}

		auto macp = worldController->getMobilePlayer();
		if (macp == nullptr) {
			throw std::runtime_error("Function called while mobile player is unavailable.");
		}

		auto animController = macp->animationController.asPlayer;
		if (animController == nullptr) {
			throw std::runtime_error("Function called while mobile player animation controller is unavailable.");
		}

		// Switch even if player is incapacitated.
		animController->switchPOVMode = 2;

		return animController->is3rdPerson;
	}

	int transferItem(sol::table params) {
		// Get the reference we are transferring from.
		TES3::Reference* fromReference = getOptionalParamReference(params, "from");
		if (fromReference == nullptr) {
			throw std::invalid_argument("Invalid 'from' parameter provided.");
		}

		// Get the reference we are transferring to.
		TES3::Reference* toReference = getOptionalParamReference(params, "to");
		if (toReference == nullptr) {
			throw std::invalid_argument("Invalid 'to' parameter provided.");
		}

		// Get the item we are going to transfer.
		TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}

		// Make sure we're dealing with actors.
		TES3::Actor* fromActor = static_cast<TES3::Actor*>(fromReference->baseObject);
		if (!fromActor->isActor()) {
			throw std::invalid_argument("The 'from' reference does not point to an actor.");
		}
		TES3::Actor* toActor = static_cast<TES3::Actor*>(toReference->baseObject);
		if (!toActor->isActor()) {
			throw std::invalid_argument("The 'to' reference does not point to an actor.");
		}

		// Do either of the references need to be cloned?
		if (fromReference->clone()) {
			fromActor = static_cast<TES3::Actor*>(fromReference->baseObject);
		}
		if (toReference->clone()) {
			toActor = static_cast<TES3::Actor*>(toReference->baseObject);
		}

		// Get any associated item data.
		TES3::ItemData* itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);

		// Get how many items we are transferring.
		int desiredCount = std::max(std::abs(getOptionalParam(params, "count", 1)), 1);
		int fulfilledCount = 0;

		// Get the mobile objects for the references, if applicable.
		auto toMobile = toReference->getAttachedMobileActor();
		auto fromMobile = fromReference->getAttachedMobileActor();

		// Are we looking at a non-container?
		auto fromIsContainer = (fromActor->objectType == TES3::ObjectType::Container);

		// Manage anything we need to regarding containers.
		float maxCapacity = -1.0f;
		float currentWeight = 0.0f;
		float itemWeight = item->getWeight();
		if (toActor->objectType == TES3::ObjectType::Container && getOptionalParam<bool>(params, "limitCapacity", true)) {
			// Prevent placing items into organic containers.
			if (BIT_TEST(toActor->actorFlags, TES3::ActorFlagContainer::OrganicBit)) {
				return 0;
			}

			// Figure out the max capacity and currently stored weight of the container.
			maxCapacity = static_cast<TES3::Container*>(toReference->getBaseObject())->capacity;
			currentWeight = toActor->inventory.calculateContainedWeight();
			if (currentWeight > maxCapacity) {
				return 0;
			}
		}

		// When transferring to the player, the OnPCAdd script variable should be set if it exists.
		auto worldController = TES3::WorldController::get();
		auto playerMobile = worldController->getMobilePlayer();
		bool onPCAdd = playerMobile && (toMobile == playerMobile);

		// Were we given an ItemData? If so, we only need to transfer one item.
		if (itemData) {
			if ((maxCapacity == -1.0f || currentWeight + itemWeight <= maxCapacity) && fromActor->inventory.containsItem(item, itemData)) {
				toActor->inventory.addItem(toMobile, item, 1, false, &itemData);
				fromActor->inventory.removeItemWithData(fromMobile, item, itemData, 1, false);

				if (!fromIsContainer) {
					fromActor->unequipItem(item, true, fromMobile, false, itemData);
				}

				// Set this item's OnPCAdd notification script variable if it exists.
				if (onPCAdd && itemData->script) {
					itemData->setScriptShortValue("OnPCAdd", 1);
				}

				fulfilledCount = 1;
				currentWeight += itemWeight;
			}
		}
		// No ItemData? We have to go through and transfer items over one by one.
		else {
			TES3::ItemStack* fromStack = fromActor->inventory.findItemStack(item);
			if (fromStack) {
				int stackCount = std::abs(fromStack->count);
				int itemsLeftToTransfer = std::min(desiredCount, stackCount);

				// If we're limited by capacity, find out how many items we really want to transfer.
				if (maxCapacity != -1.0f && itemWeight != 0.0f) {
					itemsLeftToTransfer = std::min(itemsLeftToTransfer, (int)std::floorf((maxCapacity - currentWeight) / itemWeight));
				}

				if (itemsLeftToTransfer <= 0) {
					return 0;
				}

				// Remove transfer items without data first.
				int countWithoutVariables = stackCount - (fromStack->variables ? fromStack->variables->getEndIndex() : 0);
				if (countWithoutVariables > 0) {
					int amountToTransfer = std::min(countWithoutVariables, itemsLeftToTransfer);
					toActor->inventory.addItem(toMobile, item, amountToTransfer, false, nullptr);
					fromActor->inventory.removeItemWithData(fromMobile, item, nullptr, amountToTransfer, false);

					// Check for ammunition and thrown weapons, as unlike other equipment, they do not generate itemData when equipped.
					if (!fromIsContainer && item->isWeaponOrAmmo() && static_cast<TES3::Weapon*>(item)->isProjectile()) {
						fromActor->unequipItem(item, true, fromMobile, false, nullptr);
					}

					fulfilledCount += amountToTransfer;
					itemsLeftToTransfer -= amountToTransfer;
				}

				// Then transfer over items with data.
				while (itemsLeftToTransfer > 0) {
					TES3::ItemData* itemDataRef = nullptr;

					if (fromStack->variables) {
						// We need to unequip the item first.
						auto removedEquipStack = fromActor->unequipItem(item, false, fromMobile, false, fromStack->variables->at(0));
						if (removedEquipStack == nullptr) {
							// If nothing was returned then the item wasn't equipped. So transfer using the first data.
							itemDataRef = fromStack->variables->at(0);
						}
						else {
							// Item was unequipped, but remains the first item? Preserve the item data.
							if (fromStack->variables && fromStack->variables->at(0) == removedEquipStack->itemData) {
								itemDataRef = fromStack->variables->at(0);
							}

							// Clean up after our check and manually delete.
							removedEquipStack->object = nullptr;
							removedEquipStack->itemData = nullptr;
							mwse::tes3::_delete(removedEquipStack);
							removedEquipStack = nullptr;
						}
					}

					toActor->inventory.addItem(toMobile, item, 1, false, &itemDataRef);
					fromActor->inventory.removeItemWithData(fromMobile, item, itemDataRef, 1, false);

					// Set this item's OnPCAdd notification script variable if it exists.
					if (onPCAdd && itemDataRef != nullptr && itemDataRef->script) {
						(itemDataRef)->setScriptShortValue("OnPCAdd", 1);
					}

					fulfilledCount++;
					itemsLeftToTransfer--;
				}
			}
		}

		// No items to transfer? Great, let's get out of here.
		if (fulfilledCount == 0) {
			return 0;
		}

		// Play the relevant sound.
		if (playerMobile && getOptionalParam<bool>(params, "playSound", true)) {
			if (toMobile == playerMobile) {
				worldController->playItemUpDownSound(item, TES3::ItemSoundState::Up);
			}
			else if (fromMobile == playerMobile) {
				worldController->playItemUpDownSound(item, TES3::ItemSoundState::Down);
			}
		}

		// Update equipment if needed.
		// TODO: This can be more efficient by just comparing the newly added item to the equipped item.
		if (item->promptsEquipmentReevaluation() && getOptionalParam<bool>(params, "reevaluateEquipment", true)) {
			if (toReference != playerMobile->reference) {
				toActor->reevaluateEquipment();
			}
			if (fromReference != playerMobile->reference) {
				fromActor->reevaluateEquipment();
			}
		}

		// Update body parts for creatures/NPCs that may have items unequipped.
		if (fromMobile) {
			fromReference->updateBipedParts();

			if (fromMobile == playerMobile) {
				playerMobile->firstPersonReference->updateBipedParts();
			}

			fromMobile->updateOpacity();
		}

		// If we're looking at a companion, we need to update the profit value and trigger the GUI updates.
		float isCompanion = *reinterpret_cast<float*>(0x7D3184);
		if (isCompanion != 0.0f) {
			auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
			if (contentsMenu) {
				TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
				float& companionProfit = *reinterpret_cast<float*>(0x7D3188);
				if (toReference == contentsReference) {
					companionProfit += fulfilledCount * item->getValue();
				}
				else {
					companionProfit -= fulfilledCount * item->getValue();
				}
			}
		}

		// If the item matches the target's readied ammo, update the ammo count.
		if (toMobile && item->isWeaponOrAmmo() && getOptionalParam<bool>(params, "equipProjectiles", true)) {
			auto asWeapon = static_cast<TES3::Weapon*>(item);
			if (asWeapon->isProjectile() && toMobile->readiedAmmo && toMobile->readiedAmmo->object == item) {
				toMobile->readiedAmmoCount += fulfilledCount;
			}
		}

		// If either of them are the player, we need to update the GUI.
		if (playerMobile && getOptionalParam<bool>(params, "updateGUI", true)) {
			// Update inventory menu if necessary.
			if (fromMobile == playerMobile || toMobile == playerMobile) {
				worldController->inventoryData->clearIcons(2);
				worldController->inventoryData->addInventoryItems(&playerMobile->npcInstance->inventory, 2);
				mwse::tes3::ui::inventoryUpdateIcons();
			}

			// Update contents menu if necessary.
			std::optional<float> newContainerWeight;
			auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
			if (contentsMenu) {
				// Make sure that the contents reference is one of the ones we care about.
				TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);

				// Update container weight variable.
				auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
				auto currentWeight = contentsMenu->getProperty(TES3::UI::PropertyType::Float, MenuContents_totalweight).floatValue;
				if (toReference == contentsReference) {
					newContainerWeight = currentWeight + item->getWeight() * fulfilledCount;
				}
				else {
					newContainerWeight = currentWeight - item->getWeight() * fulfilledCount;
				}
			}

			updateInventoryGUI_internal(fromReference, newContainerWeight);
			updateInventoryGUI_internal(toReference, newContainerWeight);

			// Do we need to update the magic menu?
			auto enchantment = item->getEnchantment();
			if (enchantment && (enchantment->castType == TES3::EnchantmentCastType::Once || enchantment->castType == TES3::EnchantmentCastType::OnUse)) {
				updateMagicGUI_internal(fromReference, false, true);
				updateMagicGUI_internal(toReference, false, true);
			}
		}

		fromReference->setObjectModified(true);
		toReference->setObjectModified(true);
		return fulfilledCount;
	}

	TES3::ItemData* addItemData(sol::table params) {
		// Get the reference to add the itemData to, if one was provided.
		TES3::Reference* toReference = getOptionalParamReference(params, "reference");
		if (toReference) {
			// It's a reference. This is the easy part.
			// Return nil if there is already itemData, or return the newly-created itemData.
			if (toReference->getAttachedItemData()) {
				return nullptr;
			}
			toReference->setObjectModified(true);
			return toReference->getOrCreateAttachedItemData();
		}

		// We also want to support adding itemData to the item on the cursor.
		const auto toCursor = getOptionalParam(params, "toCursor", false);
		if (toCursor) {
			const auto inventoryTile = TES3::UI::getCursorTile();
			if (inventoryTile == nullptr) {
				return nullptr;
			}

			if (inventoryTile->itemData == nullptr) {
				inventoryTile->itemData = TES3::ItemData::createForObject(inventoryTile->item);

				// Update all the other pointers to this itemData on the UI Element.
				const auto cursor = TES3::UI::getCursor();
				cursor->setProperty(TES3::UI::registerProperty("MenuInventory_extra"), inventoryTile->itemData);
				cursor->setProperty(TES3::UI::registerProperty("PartHelpMenu_extra"), inventoryTile->itemData);
			}

			return inventoryTile->itemData;
		}

		// Get the reference of the item or item container.
		toReference = getOptionalParamReference(params, "to");
		if (toReference == nullptr) {
			throw std::invalid_argument("Invalid 'to' parameter provided, or no 'reference' provided.");
		}
		else if (!toReference->baseObject->isActor()) {
			throw std::invalid_argument("Invalid 'to' parameter provided. Reference does not have an inventory. Use the 'reference' parameter to add itemData to the reference itself.");
		}

		// Get the item we are going to transfer.
		const auto item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}

		// Does the reference need to be cloned?
		toReference->clone();
		const auto inventory = toReference->getInventory();

		// Try to find an ItemData-less item and add ItemData for it.
		const auto stack = inventory->findItemStack(item);
		if (!stack || stack->count < 1) {
			throw std::runtime_error("The actor does not possess any of 'item'.");
		}

		if (!stack->variables) {
			// Create array required to hold ItemData.
			stack->variables = new NI::TArray<TES3::ItemData*>();
		}
		else if (stack->count <= int(stack->variables->getFilledCount())) {
			// All items already have ItemData.
			return nullptr;
		}

		// Finally add ItemData to stack.
		const auto itemData = TES3::ItemData::createForObject(item);
		stack->variables->add(itemData);

		// If either of them are the player, we need to update the GUI.
		auto worldController = TES3::WorldController::get();
		auto toMobile = toReference->getAttachedMobileActor();
		auto playerMobile = worldController->getMobilePlayer();

		if (getOptionalParam<bool>(params, "updateGUI", true)) {
			updateInventoryGUI_internal(toReference);

			// Do we need to update the magic menu?
			auto enchantment = item->getEnchantment();
			if (enchantment && (enchantment->castType == TES3::EnchantmentCastType::Once || enchantment->castType == TES3::EnchantmentCastType::OnUse)) {
				updateMagicGUI_internal(toReference, false, true);
			}
		}

		toReference->setObjectModified(true);
		return itemData;
	}

	bool removeItemData(sol::table params) {
		auto forceRemoval = getOptionalParam(params, "force", false);
		auto ignoreOwnership = getOptionalParam(params, "ignoreOwnership", true);

		// Get the reference of the item or item container.
		auto reference = getOptionalParamReference(params, "from");
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'from' parameter provided.");
		}

		// Check if it's setting a world reference or an item in a container.
		auto referenceActor = static_cast<TES3::Actor*>(reference->baseObject);
		if (!referenceActor->isActor()) {
			// It's a reference. This is the easy part.
			auto itemData = reference->getAttachedItemData();
			if (!itemData) {
				return false;
			}

			// Check to see if the item is fully repaired.
			if (!forceRemoval && !TES3::ItemData::isItemDataStackable(itemData, static_cast<TES3::Item*>(reference->baseObject), ignoreOwnership)) {
				return false;
			}

			// Detach the attachment and delete the associated data.
			reference->removeAttachment(reference->getAttachment(TES3::AttachmentType::Variables));
			tes3::_delete(itemData);
			reference->setObjectModified(true);
			return true;
		}

		// Get the item that relates to the item data.
		auto item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}

		// Get the item data to remove.
		auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'itemData' parameter provided.");
		}

		// We don't want to remove it if the item is equipped.
		auto equipped = referenceActor->getEquippedItemExact(item, itemData);
		if (equipped) {
			return false;
		}

		// Do we need to remove it?
		if (!forceRemoval && !TES3::ItemData::isItemDataStackable(itemData, item, ignoreOwnership)) {
			return false;
		}

		// Check to see if the itemData actually exists for this item.
		auto stack = referenceActor->inventory.findItemStack(item);
		if (!stack || !stack->variables || !stack->variables->contains(itemData)) {
			return false;
		}

		// Actually remove and delete the item data.
		referenceActor->inventory.removeItemData(item, itemData);

		// Update the GUI.
		if (getOptionalParam<bool>(params, "updateGUI", true)) {
			updateInventoryGUI_internal(reference);

			// Do we need to update the magic menu?
			auto enchantment = item->getEnchantment();
			if (enchantment && (enchantment->castType == TES3::EnchantmentCastType::Once || enchantment->castType == TES3::EnchantmentCastType::OnUse)) {
				updateMagicGUI_internal(reference, false, true);
			}
		}

		reference->baseObject->setObjectModified(true);
		reference->setObjectModified(true);

		return true;
	}

	int getCurrentAIPackageId(sol::object params) {
		TES3::Reference* refr = nullptr;
		TES3::MobileActor* mobileActor = nullptr;

		if (params.is<TES3::Reference*>()) {
			// Legacy param handling where the param is a reference, not a table.
			refr = params.as<TES3::Reference*>();
			mobileActor = refr->getAttachedMobileActor();
		}
		else if (params.is<sol::table>()) {
			// Standard param handling.
			auto paramsTable = params.as<sol::table>();
			refr = getOptionalParamReference(paramsTable, "reference");
			mobileActor = getOptionalParamMobileActor(paramsTable, "reference");
		}

		if (mobileActor) {
			if (mobileActor->aiPlanner != nullptr) {
				auto currentPackage = mobileActor->aiPlanner->getActivePackage();
				if (currentPackage != nullptr) {
					return int(currentPackage->packageType);
				}
			}
		}
		else if (refr && refr->baseObject->isActor()) {
			TES3::Actor* actor = static_cast<TES3::Actor*>(refr->baseObject);
			auto currentConfig = actor->getAIPackageConfig();
			if (currentConfig != nullptr) {
				// Convert config type to AIPackageType
				return int(currentConfig->toPackageType());
			}
		}
		else {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		return -1;
	}

	void setAIActivate(sol::table params) {
		auto mobileActor = getOptionalParamMobileActor(params, "reference");
		if (mobileActor == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		auto target = getOptionalParamReference(params, "target");
		if (target == nullptr) {
			throw std::invalid_argument("Invalid target parameter provided.");
		}

		auto config = new TES3::AIPackageActivate::Config();
		config->target = target;
		config->reset = getOptionalParam<bool>(params, "reset", true);

		auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
		actor->setAIPackage(config, mobileActor->reference);
	}

	void setAIFollow(sol::table params) {
		auto mobileActor = getOptionalParamMobileActor(params, "reference");
		if (mobileActor == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		auto target = getOptionalParamReference(params, "target");
		if (target == nullptr || !target->baseObject->isActor()) {
			throw std::invalid_argument("Invalid target parameter provided.");
		}

		auto destination = getOptionalParamVector3(params, "destination");

		auto config = new TES3::AIPackageFollow::Config();
		if (destination) {
			config->destination = destination.value();
		}
		else {
			config->destination = TES3::Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.0f);
		}
		config->duration = getOptionalParam<unsigned char>(params, "duration", 0);
		config->actor = static_cast<TES3::Actor*>(target->baseObject);
		config->cell = getOptionalParamCell(params, "cell");
		config->reset = getOptionalParam<bool>(params, "reset", true);

		auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
		actor->setAIPackage(config, mobileActor->reference);
	}

	void setAIEscort(sol::table params) {
		TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
		if (mobileActor == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		TES3::Reference* target = getOptionalParamReference(params, "target");
		if (target == nullptr || !target->baseObject->isActor()) {
			throw std::invalid_argument("Invalid target parameter provided.");
		}

		auto destination = getOptionalParamVector3(params, "destination");
		if (!destination) {
			throw std::invalid_argument("Destination parameter is missing.");
		}

		auto config = new TES3::AIPackageEscort::Config();
		config->destination = destination.value();
		config->duration = getOptionalParam<unsigned char>(params, "duration", 0);
		config->actor = static_cast<TES3::Actor*>(target->getBaseObject());
		config->cell = getOptionalParamCell(params, "cell");
		config->reset = getOptionalParam<bool>(params, "reset", true);

		auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
		actor->setAIPackage(config, mobileActor->reference);
	}

	void setAITravel(sol::table params) {
		auto mobileActor = getOptionalParamMobileActor(params, "reference");
		if (mobileActor == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		auto destination = getOptionalParamVector3(params, "destination");
		if (!destination) {
			throw std::invalid_argument("Invalid destination parameter provided.");
		}

		auto config = new TES3::AIPackageTravel::Config();
		config->position = destination.value();
		config->reset = getOptionalParam<bool>(params, "reset", true);

		auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
		actor->setAIPackage(config, mobileActor->reference);
	}

	void setAIWander(sol::table params) {
		auto mobileActor = getOptionalParamMobileActor(params, "reference");
		if (mobileActor == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		sol::optional<sol::table> maybeIdles = params["idles"];
		if (!maybeIdles || maybeIdles.value().get_type() != sol::type::table) {
			throw std::invalid_argument("Invalid idles table provided.");
		}

		auto config = new TES3::AIPackageWander::Config();
		config->range = getOptionalParam<unsigned short>(params, "range", 0);
		config->duration = getOptionalParam<unsigned char>(params, "duration", 0);
		config->time = getOptionalParam<unsigned char>(params, "time", 0);
		config->reset = getOptionalParam<bool>(params, "reset", true);

		sol::table idles = maybeIdles.value();
		for (size_t i = 0; i < 8; ++i) {
			config->idles[i] = idles.get_or(i + 1, 0);
		}

		auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
		actor->setAIPackage(config, mobileActor->reference);
	}

	void say(sol::table params) {
		auto dataHandler = TES3::DataHandler::get();
		auto worldController = TES3::WorldController::get();
		if (worldController == nullptr || dataHandler == nullptr) {
			throw std::invalid_argument("Cannoy be called before tes3worldController and tes3dataHandler are initialized.");
		}

		auto reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		const char* path = getOptionalParam<const char*>(params, "soundPath", nullptr);
		if (path == nullptr) {
			throw std::invalid_argument("Invalid soundPath parameter provided.");
		}

		auto pitch = getOptionalParam(params, "pitch", 1.0f);

		// Apply volume, using mix channel and rescale to 0-250.
		auto volume = std::clamp(getOptionalParam(params, "volume", 1.0), 0.0, 1.0);
		volume *= 250.0 * worldController->audioController->getMixVolume(TES3::AudioMixType::Voice);

		// Show a messagebox.
		if (worldController->showSubtitles || getOptionalParam(params, "forceSubtitle", false)) {
			const char* subtitle = getOptionalParam<const char*>(params, "subtitle", nullptr);
			if (subtitle != nullptr) {
				TES3::UI::showMessageBox(subtitle);
			}
		}

		// Play the related sound.
		dataHandler->addTemporarySound(path, reference, 0, int(volume), pitch, true);
	}

	bool hasOwnershipAccess(sol::table params) {
		// Who are we checking ownership for? The player by default.
		auto reference = getOptionalParamExecutionReference(params);
		auto playerReference = TES3::WorldController::get()->getMobilePlayer()->reference;
		if (reference == nullptr) {
			reference = playerReference;
		}
		auto referenceBaseObject = reference->getBaseObject();

		// What are we checking ownership of?
		auto target = getOptionalParamReference(params, "target");
		if (target == nullptr) {
			throw std::invalid_argument("Invalid target parameter provided.");
		}

		// You always have access to yourself.
		if (reference == target) {
			return true;
		}

		// The player never has access to living actors.
		auto targetBaseObject = target->getBaseObject();
		if (target->isActor() && !target->isDead().value_or(true)) {
			return false;
		}

		// Do we have an owner?
		auto targetData = target->getAttachedItemData();
		if (targetData == nullptr || targetData->owner == nullptr) {
			return true;
		}

		// Are we looking at an NPC owner?
		if (targetData->owner->objectType == TES3::ObjectType::NPC) {
			// We own our own things.
			if (referenceBaseObject == targetData->owner) {
				return true;
			}

			// Is the target dead? We have to check the kill counter here.
			else if (reference == playerReference && TES3::WorldController::get()->playerKills->getKillCount(static_cast<TES3::NPC*>(targetData->owner)) > 0) {
				return true;
			}

			// No variable? No chance to own.
			else if (targetData->requiredVariable == nullptr) {
				return false;
			}

			// Players get access if the variable is set.
			else if (reference == playerReference) {
				return targetData->requiredVariable->value > 0.0f;
			}
		}
		// How about a faction?
		else if (targetData->owner->objectType == TES3::ObjectType::Faction) {
			auto ownerAsFaction = reinterpret_cast<TES3::Faction*>(targetData->owner);

			// Players require the right rank.
			if (reference == playerReference) {
				return ownerAsFaction->getEffectivePlayerRank() >= targetData->requiredRank;
			}

			// As do NPCs, but their faction info is stored elsewhere.
			auto referenceObjectAsNPC = reinterpret_cast<TES3::NPC*>(reference->getBaseObject());
			if (referenceObjectAsNPC->objectType == TES3::ObjectType::NPC) {
				return (referenceObjectAsNPC->getFaction() == targetData->owner && referenceObjectAsNPC->factionRank >= targetData->requiredRank);
			}
		}

		return false;
	}

	TES3::Reference* dropItem(sol::table params) {
		// Who is dropping?
		auto mobile = getOptionalParamMobileActor(params, "reference");
		if (mobile == nullptr) {
			throw std::invalid_argument("Invalid reference parameter provided.");
		}

		// What are they dropping?
		auto item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid item parameter provided.");
		}

		// Get data about what is being dropped.
		auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData");
		auto count = getOptionalParam<int>(params, "count", 1);
		auto matchNoItemData = getOptionalParam<bool>(params, "matchNoItemData", false);

		// Check if the item to drop exists, as the game functions don't handle it.
		auto actor = static_cast<TES3::Actor*>(mobile->reference->baseObject);
		auto stack = actor->inventory.findItemStack(item);
		if (!stack) {
			return nullptr;
		}
		if (matchNoItemData) {
			if (stack->variables && stack->count == stack->variables->filledCount) {
				// Match failed: All items have itemData.
				return nullptr;
			}
		}
		else if (itemData.has_value()) {
			if (stack->variables == nullptr || !stack->variables->contains(itemData.value())) {
				// Match failed: itemData is not found.
				return nullptr;
			}
		}

		TES3::WorldController::ItemUpDownSoundBlocker soundBlocker = !getOptionalParam<bool>(params, "playSound", true);

		// Drop the item.
		auto matchExact = itemData.has_value() || matchNoItemData;
		mobile->dropItem(item, itemData.value_or(nullptr), count, !matchExact);
		auto droppedReference = mobile->getCell()->temporaryRefs.tail;

		// Update inventory tiles if needed.
		if (getOptionalParam<bool>(params, "updateGUI", true)) {
			updateInventoryGUI_internal(mobile->reference);

			// Do we need to update the magic menu?
			auto enchantment = item->getEnchantment();
			if (enchantment && (enchantment->castType == TES3::EnchantmentCastType::Once || enchantment->castType == TES3::EnchantmentCastType::OnUse)) {
				updateMagicGUI_internal(mobile->reference, false, true);
			}
		}

		return droppedReference;
	}

	bool equip(sol::optional<sol::table> params) {
		const auto reference = getOptionalParamReference(params, "reference");
		if (!reference) {
			throw std::runtime_error("Invalid 'reference' parameter provided.");
		}
		else if (!reference->baseObject->isActor()) {
			throw std::runtime_error("Invalid 'reference' parameter provided. It does not have an inventory.");
		}

		const auto mobile = reference->getAttachedMobileActor();
		if (mobile == nullptr) {
			return false;
		}

		const auto item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			return false;
		}

		const auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
		const auto addItem = getOptionalParam<bool>(params, "addItem", false);
		const auto selectBestCondition = getOptionalParam<bool>(params, "selectBestCondition", false);
		const auto selectWorstCondition = getOptionalParam<bool>(params, "selectWorstCondition", false);
		const auto bypassEquipEvents = getOptionalParam<bool>(params, "bypassEquipEvents", false);
		TES3::WorldController::ItemUpDownSoundBlocker soundBlocker = !getOptionalParam<bool>(params, "playSound", true);

		return mobile->equipItem(item, itemData, addItem, selectBestCondition, selectWorstCondition, !bypassEquipEvents);
	}

	bool persuade(sol::table params) {
		auto actor = getOptionalParamMobileActor(params, "actor");
		if (actor == nullptr) {
			throw std::invalid_argument("Invalid actor parameter provided.");
		}

		auto rng = tes3::rand() % 100;

		auto index = getOptionalParam<int>(params, "index", -1);

		if (index >= 0 && index <= 6) {
			actor->reference->setObjectModified(true);
			return actor->persuade(rng, index);
		}
		else {
			auto fBribe100Mod = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fBribe100Mod];
			auto modifier = getOptionalParam<float>(params, "modifier", 0.0f);

			auto oldModifier = fBribe100Mod->value.asFloat;
			fBribe100Mod->value.asFloat = modifier;

			auto result = actor->persuade(rng, 4);
			fBribe100Mod->value.asFloat = oldModifier;

			actor->reference->setObjectModified(true);

			return result;
		}
	}

	TES3::Reference* findClosestExteriorReferenceOfObject(sol::table params) {
		auto dataHandler = TES3::DataHandler::get();

		auto object = getOptionalParamObject<TES3::PhysicalObject>(params, "object");
		if (object == nullptr) {
			throw std::invalid_argument("Invalid 'object' parameter provided.");
		}

		sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
		if (!position) {
			position = dataHandler->getLastExteriorPosition();
		}

		if (dataHandler) {
			return dataHandler->nonDynamicData->findClosestExteriorReferenceOfObject(object, &position.value());
		}
		return nullptr;
	}

	TES3::Dialogue* findDialogue(sol::table params) {
		auto topic = getOptionalParam<const char*>(params, "topic", nullptr);
		if (topic) {
			return TES3::DataHandler::get()->nonDynamicData->findDialogue(topic);
		}

		auto type = getOptionalParam<int>(params, "type", -1);
		auto page = getOptionalParam<int>(params, "page", -1);
		return TES3::Dialogue::getDialogue(type, page);
	}

	TES3::Quest* findQuest(sol::table params) {
		auto journalController = TES3::WorldController::get()->journalController;

		sol::object journal = params["journal"];
		TES3::Dialogue* dialogue = nullptr;

		if (journal.is<TES3::Dialogue*>()) {
			dialogue = journal.as<TES3::Dialogue*>();
		}
		else if (journal.is<const char*>()) {
			auto dialogueId = journal.as<const char*>();
			if (dialogueId) {
				dialogue = TES3::DataHandler::get()->nonDynamicData->findDialogue(dialogueId);
			}
		}

		if (dialogue) {
			// Search by dialogue topic.
			for (auto quest : *journalController) {
				for (auto d : quest->dialogues) {
					if (d == dialogue) {
						return quest;
					}
				}
			}
			return nullptr;
		}

		auto name = getOptionalParam<const char*>(params, "name", nullptr);
		if (name) {
			// Search by quest name string.
			for (auto quest : *journalController) {
				if (_stricmp(name, quest->name) == 0) {
					return quest;
				}
			}
			return nullptr;
		}

		return nullptr;
	}

	bool setEnabled(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		reference->setObjectModified(true);

		// Allow toggling.
		if (getOptionalParam<bool>(params, "toggle", false)) {
			if (reference->getDisabled()) {
				return reference->enable();
			}
			else {
				return reference->disable();
			}
		}
		// Otherwise base it on enabled (default true).
		else {
			if (getOptionalParam<bool>(params, "enabled", true)) {
				return reference->enable();
			}
			else {
				return reference->disable();
			}
		}
	}

	void setExpelled(sol::table params) {
		TES3::Faction* faction = getOptionalParamObject<TES3::Faction>(params, "faction");
		if (faction == nullptr) {
			throw std::invalid_argument("Invalid 'faction' parameter provided");
		}

		if (getOptionalParam<bool>(params, "expelled", true)) {
			faction->setPlayerExpelled(true);
			std::string message = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sExpelledMessage]->value.asString;
			message.append(faction->getName());
			TES3::UI::showMessageBox(message.c_str(), nullptr, true);
		}
		else {
			faction->setPlayerExpelled(false);
		}
	}

	sol::optional<std::tuple<unsigned char, unsigned char, unsigned char>> getCurrentAnimationGroups(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (animData == nullptr) {
			return {};
		}

		return std::make_tuple(animData->currentAnimGroup[0], animData->currentAnimGroup[1], animData->currentAnimGroup[2]);
	}

	void loadAnimation(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (reference->sceneNode == nullptr || animData == nullptr) {
			return;
		}

		// Change the animation temporarily. Passing nullptr resets the animation to base.
		const char* modelFile = getOptionalParam<const char*>(params, "file", nullptr);

		if (modelFile) {
			reference->setModelPath(modelFile, true);

			animData = reference->getAttachedAnimationData();
			if (!animData->hasOverrideAnimations()) {
				throw std::logic_error("Animation file failed to load.");
			}
		}
		else {
			reference->reloadAnimation(reference->baseObject->getModelPath());

			// Reset animation control that may be set by playAnimation.
			auto mact = reference->getAttachedMobileActor();
			if (mact) {
				mact->setMobileActorFlag(TES3::MobileActorFlag::IdleAnim, false);
			}
		}
	}

	void playAnimation(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		// Ensure there is target data to override.
		auto animData = reference->getAttachedAnimationData();
		if (reference->sceneNode == nullptr || animData == nullptr) {
			return;
		}

		// Allow loading an animation temporarily.
		const char* modelFile = getOptionalParam<const char*>(params, "mesh", nullptr);
		if (modelFile != nullptr) {
			reference->setModelPath(modelFile, true);
			animData = reference->getAttachedAnimationData();
		}

		int group = getOptionalParam<int>(params, "group", -1);
		if (group < -1 || group > 149) {
			throw std::invalid_argument("Invalid 'group' parameter provided: must be between 0 and 149.");
		}

		int lowerGroup = getOptionalParam<int>(params, "lower", group);
		if (lowerGroup < -1 || lowerGroup > 149) {
			throw std::invalid_argument("Invalid 'lowerGroup' parameter provided: must be between 0 and 149.");
		}

		int upperGroup = getOptionalParam<int>(params, "upper", lowerGroup);
		if (upperGroup < -1 || upperGroup > 149) {
			throw std::invalid_argument("Invalid 'upperGroup' parameter provided: must be between 0 and 149.");
		}

		int shieldGroup = getOptionalParam<int>(params, "shield", upperGroup);
		if (shieldGroup < -1 || shieldGroup > 149) {
			throw std::invalid_argument("Invalid 'shieldGroup' parameter provided: must be between 0 and 149.");
		}

		// Play anim group 0 (returns control to AI) if no groups are specified.
		if (lowerGroup == -1 && upperGroup == -1 && shieldGroup == -1) {
			group = lowerGroup = upperGroup = shieldGroup = 0;
		}

		// Default to immediate start and infinite looping.
		int startFlag = getOptionalParam<int>(params, "startFlag", 1);
		int loopCount = getOptionalParam<int>(params, "loopCount", -1);

		// Start animations.
		if (lowerGroup != -1) {
			animData->playAnimationGroupForIndex(lowerGroup, 0, startFlag, loopCount);
		}
		if (upperGroup != -1) {
			animData->playAnimationGroupForIndex(upperGroup, 1, startFlag, loopCount);
		}
		if (shieldGroup != -1) {
			animData->playAnimationGroupForIndex(shieldGroup, 2, startFlag, loopCount);
		}

		auto mact = reference->getAttachedMobileActor();
		if (mact) {
			// If no overall group is specified, do not idle AI and only override specified body part groups.
			if (group == -1) {
				if (mact->animationController.asActor) {
					unsigned char targetBones = 0xFF;
					if (lowerGroup != -1) {
						targetBones = 0;
					}
					else if (upperGroup != -1) {
						targetBones = 1;
					}
					else if (shieldGroup != -1) {
						targetBones = 2;
					}
					mact->animationController.asActor->patchedOverrideState = targetBones;
				}
			}
			else {
				// Idle anim flag pauses all AI animation control.
				bool idleAnim = getOptionalParam<bool>(params, "idleAnim", lowerGroup > 0 || upperGroup > 0 || shieldGroup > 0);
				mact->setMobileActorFlag(TES3::MobileActorFlag::IdleAnim, idleAnim);
				if (mact->animationController.asActor) {
					mact->animationController.asActor->patchedOverrideState = 0xFF;
				}
			}
		}
	}

	void cancelAnimationLoop(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (reference->sceneNode == nullptr || animData == nullptr) {
			return;
		}

		if (animData->currentAnimGroup[0] != 0) {
			animData->loopCounts[0] = 0;
		}
		if (animData->currentAnimGroup[1] != 0) {
			animData->loopCounts[1] = 0;
		}
		if (animData->currentAnimGroup[2] != 0) {
			animData->loopCounts[2] = 0;
		}
	}

	void skipAnimationFrame(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (animData == nullptr) {
			return;
		}

		animData->flags |= 0xFFFF;
	}

	sol::optional<sol::table> getAnimationTiming(sol::table params, sol::this_state thisState) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (animData == nullptr) {
			return {};
		}

		sol::state_view state = thisState;
		sol::table result = state.create_table();
		for (size_t i = 0; i < 3; ++i) {
			result[i + 1] = animData->timing[i];
		}
		return result;
	}

	void setAnimationTiming(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (animData == nullptr) {
			return;
		}

		sol::object timing = params["timing"];
		if (timing.valid() && timing != sol::nil) {
			if (timing.is<float>()) {
				const float fTiming = timing.as<float>();
				animData->timing[0] = fTiming;
				animData->timing[1] = fTiming;
				animData->timing[2] = fTiming;
			}
			else if (timing.is<sol::table>() && timing.as<sol::table>().size() == 3) {
				auto timings = timing.as<sol::table>();
				animData->timing[0] = timings[1];
				animData->timing[1] = timings[2];
				animData->timing[2] = timings[3];
			}
		}
	}

	sol::optional<sol::table> getAnimationActionTiming(sol::table params, sol::this_state thisState) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		int group = getOptionalParam<int>(params, "group", -1);
		if (group < -1 || group > 149) {
			throw std::invalid_argument("Invalid 'group' parameter provided: must be between 0 and 149.");
		}

		auto animData = reference->getAttachedAnimationData();
		if (animData == nullptr) {
			return {};
		}

		auto animGroup = animData->animationGroups[group];
		if (animGroup == nullptr) {
			return {};
		}

		const auto animNoteTextByClass = reinterpret_cast<const char**>(0x78ABC8);
		const auto animGroupNoteClass = reinterpret_cast<const int*>(0x78B0A8);
		auto noteLabel = animNoteTextByClass + animGroupNoteClass[group];

		sol::state_view state = thisState;
		sol::table result = state.create_table();
		for (size_t i = 0; i < animGroup->actionCount; ++i, noteLabel += 8) {
			result[*noteLabel] = animGroup->actionTimings[i];
		}
		return result;
	}

	bool isAffectedBy(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto mact = reference->getAttachedMobileActor();
		if (mact == nullptr || !mact->isActor()) {
			throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
		}

		// Are we checking for being affected by an object?
		int effectId = getOptionalParam<int>(params, "effect", -1);
		TES3::BaseObject* object = getOptionalParamObject<TES3::BaseObject>(params, "object");
		if (object != nullptr) {
			if (object->objectType == TES3::ObjectType::Alchemy) {
				return mact->isAffectedBySimilarAlchemy(static_cast<TES3::Alchemy*>(object));
			}
			else if (object->objectType == TES3::ObjectType::Enchantment) {
				return mact->isAffectedByEnchantment(static_cast<TES3::Enchantment*>(object));
			}
			else if (object->objectType == TES3::ObjectType::Spell) {
				return mact->isAffectedBySpell(static_cast<TES3::Spell*>(object));
			}
			else if (object->objectType == TES3::ObjectType::MagicEffect) {
				effectId = static_cast<TES3::MagicEffect*>(object)->id;
			}
			else {
				throw std::invalid_argument("Invalid 'object' parameter provided.");
			}
		}

		// Check based on effect ID.
		if (effectId > -1) {
			auto effectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;
			sol::optional<int> skillOrAttributeID;
			if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetAttributeBit)) {
				skillOrAttributeID = getOptionalParam<int>(params, "attribute");
				if (!skillOrAttributeID) {
					throw std::invalid_argument("Invalid 'attribute' parameter provided. The given effect requires an associated attribute.");
				}
			}
			else if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetSkillBit)) {
				skillOrAttributeID = getOptionalParam<int>(params, "skill");
				if (!skillOrAttributeID) {
					throw std::invalid_argument("Invalid 'skill' parameter provided. The given effect requires an associated skill.");
				}
			}

			for (auto& activeEffect : mact->activeMagicEffects) {
				if (activeEffect.magicEffectID == effectId && (!skillOrAttributeID || activeEffect.skillOrAttributeID == skillOrAttributeID)) {
					return true;
				}
			}
		}
		else {
			throw std::invalid_argument("Invalid 'effect' parameter provided.");
		}

		return false;
	}

	std::pair<float, unsigned int> getEffectMagnitude(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto mact = reference->getAttachedMobileActor();
		if (mact == nullptr || !mact->isActor()) {
			throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
		}

		int effectId = getOptionalParam<int>(params, "effect", -1);
		auto effectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;
		if (!effectController->getEffectExists(effectId)) {
			throw std::invalid_argument("Invalid 'effect' parameter provided. No effect exists with the given id.");
		}

		if (effectController->getEffectFlag(effectId, TES3::EffectFlag::NoMagnitudeBit)) {
			return std::make_pair(0.0f, 0);
		}

		// Get our attribute or skill, if appropriate.
		sol::optional<int> skillOrAttributeID;
		if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetAttributeBit)) {
			skillOrAttributeID = getOptionalParam<int>(params, "attribute");
			if (!skillOrAttributeID) {
				return std::make_pair(0.0f, 0);
			}
		}
		else if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetSkillBit)) {
			skillOrAttributeID = getOptionalParam<int>(params, "skill");
			if (!skillOrAttributeID) {
				return std::make_pair(0.0f, 0);
			}
		}

		auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
		auto referenceID = reference->getObjectID();
		unsigned int magnitude = 0;
		float effectiveMagnitude = 0;

		for (auto& activeEffect : mact->activeMagicEffects) {
			if (activeEffect.magicEffectID == effectId && (!skillOrAttributeID || activeEffect.skillOrAttributeID == skillOrAttributeID)) {
				auto instance = magicInstanceController->getInstanceFromSerial(activeEffect.magicInstanceSerial);
				if (instance) {
					auto effectInstance = instance->effects[activeEffect.magicInstanceEffectIndex].getNode(referenceID);
					if (effectInstance) {
						magnitude += effectInstance->value.magnitude;
						effectiveMagnitude += effectInstance->value.getEffectiveMagnitude();
					}
				}
			}
		}
		return std::make_pair(effectiveMagnitude, magnitude);
	}

	TES3::MagicEffect* addMagicEffect(sol::table params) {
		auto magicEffectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;

		// Get the new ID.
		int id = getOptionalParam<int>(params, "id", -1);;
		if (id <= TES3::EffectID::LastEffect || id >= TES3::EFFECT_ID_INVALID) {
			throw std::invalid_argument("Invalid 'id' parameter provided. It must be an integer greater than 142 and less than 32766.");
		}
		else if (magicEffectController->getEffectExists(id)) {
			throw std::invalid_argument("Invalid 'id' parameter provided. An effect with this id already exists.");
		}

		// Create the effect and assign basic properties.
		auto effect = new TES3::MagicEffect(id);
		effect->baseMagickaCost = getOptionalParam<float>(params, "baseCost", 1.0f);
		effect->school = getOptionalParam<int>(params, "school", 0);
		effect->size = getOptionalParam<float>(params, "size", 1.0f);
		effect->sizeCap = getOptionalParam<float>(params, "sizeCap", 1.0f);
		effect->speed = getOptionalParam<float>(params, "speed", 1.0f);

		// Description.
		sol::optional<std::string> description = params["description"];
		if (description) {
			effect->setDescription(description.value().c_str());
		}
		else {
			effect->setDescription("No description available.");
		}

		// Set color.
		auto lighting = getOptionalParamVector3(params, "lighting");
		if (lighting) {
			effect->lightingRed = int(std::clamp(lighting.value().x, 0.0f, 1.0f) * 255);
			effect->lightingGreen = int(std::clamp(lighting.value().y, 0.0f, 1.0f) * 255);
			effect->lightingBlue = int(std::clamp(lighting.value().z, 0.0f, 1.0f) * 255);
		}
		else {
			effect->lightingRed = 255;
			effect->lightingGreen = 255;
			effect->lightingBlue = 255;
		}

		sol::optional<std::string> icon = params["icon"];
		if (!icon || icon.value().length() >= 31) {
			delete effect;
			throw std::invalid_argument("Invalid 'icon' parameter provided. Must be a string no longer than 31 characters long.");
		}
		else {
			strcpy_s(effect->icon, 32, icon.value().c_str());
		}

		sol::optional<std::string> particleTexture = params["particleTexture"];
		if (!particleTexture || particleTexture.value().length() >= 31) {
			delete effect;
			throw std::invalid_argument("Invalid 'particleTexture' parameter provided. Must be a string no longer than 31 characters long.");
		}
		else {
			strcpy_s(effect->particleTexture, 32, particleTexture.value().c_str());
		}

		sol::optional<std::string> castSound = params["castSound"];
		if (castSound) {
			if (castSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'castSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			strcpy_s(effect->castSoundEffectID, 32, castSound.value().c_str());
		}

		sol::optional<std::string> boltSound = params["boltSound"];
		if (boltSound) {
			if (boltSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'boltSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			strcpy_s(effect->boltSoundEffectID, 32, boltSound.value().c_str());
		}

		sol::optional<std::string> hitSound = params["hitSound"];
		if (hitSound) {
			if (hitSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'hitSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			strcpy_s(effect->hitSoundEffectID, 32, hitSound.value().c_str());
		}

		sol::optional<std::string> areaSound = params["areaSound"];
		if (areaSound) {
			if (areaSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'areaSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			strcpy_s(effect->areaSoundEffectID, 32, areaSound.value().c_str());
		}

		effect->castEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "castVFX");
		effect->boltEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "boltVFX");
		effect->hitEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "hitVFX");
		effect->areaEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "areaVFX");

		auto& extendedData = magicEffectController->effectExtendedData[id];
		if (extendedData == nullptr) {
			extendedData = new TES3::MagicEffectExtendedData();
		}

		sol::optional<std::string> name = params["name"];
		if (name) {
			extendedData->name = name.value();
		}

		sol::optional<std::string> magnitudeType = params["magnitudeType"];
		if (magnitudeType) {
			extendedData->magnitudeType = magnitudeType.value();

			sol::optional<std::string> magnitudeTypePlural = params["magnitudeTypePlural"];
			if (magnitudeTypePlural) {
				extendedData->magnitudeTypePlural = magnitudeTypePlural.value();
			}
		}

		// Actually add the effect.
		magicEffectController->addEffectObject(effect);

		// Set individual flags.
		magicEffectController->setEffectFlags(id, 0U);
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::AllowEnchantingBit,
			getOptionalParam<bool>(params, "allowEnchanting", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::AllowSpellmakingBit,
			getOptionalParam<bool>(params, "allowSpellmaking", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::AppliedOnceBit,
			getOptionalParam<bool>(params, "appliesOnce", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::CanCastSelfBit,
			getOptionalParam<bool>(params, "canCastSelf", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::CanCastTargetBit,
			getOptionalParam<bool>(params, "canCastTarget", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::CanCastTouchBit,
			getOptionalParam<bool>(params, "canCastTouch", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::CasterLinkedBit,
			getOptionalParam<bool>(params, "casterLinked", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::ContinuousVFXBit,
			getOptionalParam<bool>(params, "hasContinuousVFX", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::NoDurationBit,
			getOptionalParam<bool>(params, "hasNoDuration", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::NoMagnitudeBit,
			getOptionalParam<bool>(params, "hasNoMagnitude", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::IllegalDaedraBit,
			getOptionalParam<bool>(params, "illegalDaedra", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::HarmfulBit,
			getOptionalParam<bool>(params, "isHarmful", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::NonRecastableBit,
			getOptionalParam<bool>(params, "nonRecastable", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::TargetAttributeBit,
			getOptionalParam<bool>(params, "targetsAttributes", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::TargetSkillBit,
			getOptionalParam<bool>(params, "targetsSkills", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::UnreflectableBit,
			getOptionalParam<bool>(params, "unreflectable", true));
		magicEffectController->setEffectFlag(id, TES3::EffectFlag::NegativeLightingBit,
			getOptionalParam<bool>(params, "usesNegativeLighting", true));
		effect->flags = magicEffectController->getEffectFlags(id);

		// Get the tick function.
		sol::optional<sol::protected_function> onTick = params["onTick"];
		if (onTick) {
			extendedData->tickFunction = onTick.value();
		}
		sol::optional<sol::protected_function> onCollision = params["onCollision"];
		if (onCollision) {
			extendedData->collisionFunction = onCollision.value();
		}

		// Set the GMST as the negative effect ID for custom hooks later.
		magicEffectController->effectNameGMSTs[id] = -id;

		return effect;
	}

	double advanceTime(sol::table params) {
		float rawHours = getOptionalParam(params, "hours", 0.0f);
		float hoursPassed = 0.0f;

		// Leverage the resting/waiting system if we're advancing by an hour or more.
		if (rawHours >= 1.0) {
			int hoursToAdvance = int(std::floor(rawHours));

			auto worldController = TES3::WorldController::get();
			worldController->gvarGameHour->value += rawHours - hoursToAdvance;
			hoursPassed += rawHours - hoursToAdvance;

			bool resting = getOptionalParam<bool>(params, "resting", false);
			bool updateEnvironment = getOptionalParam<bool>(params, "updateEnvironment", true);

			auto macp = worldController->getMobilePlayer();
			macp->restHoursRemaining = hoursToAdvance;
			macp->sleeping = resting;
			macp->waiting = !resting;

			while (macp->restHoursRemaining > 0) {
				reinterpret_cast<void(__cdecl*)(bool)>(0x6350B0)(updateEnvironment);
				hoursPassed += 1;
			}
		}
		// Otherwise, just do the bare minimum.
		else {
			auto worldController = TES3::WorldController::get();
			worldController->gvarGameHour->value += rawHours;
			worldController->checkForDayWrapping();
			worldController->updateEnvironmentLightingWeather();
			worldController->processGlobalScripts();
			hoursPassed += rawHours;
		}

		return hoursPassed;
	}

	// WARNING: This function is not yet complete.
	bool beginTransform(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto mact = reference->getAttachedMobileActor();
		if (mact == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
		}

		if (mact->getIsWerewolf()) {
			return false;
		}

		if (mact->isDead()) {
			return false;
		}

		mact->changeWerewolfState(true);
		mact->setMobileActorFlag(TES3::MobileActorFlag::BodypartsChanged, false);

		auto macp = TES3::WorldController::get()->getMobilePlayer();
		if (mact == macp) {
			auto worldController = TES3::WorldController::get();

			// Change FOV.
			worldController->shadowCamera.cameraData.setFOV(float(worldController->werewolfFOV));
			worldController->worldCamera.cameraData.setFOV(float(worldController->werewolfFOV));

			// Update faders.
			worldController->werewolfFader->updateMaterialProperty(1.0f);
			worldController->sunglareFader->deactivate();
			worldController->werewolfFader->activate();
		}

		return true;
	}

	bool undoTransform(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto mact = reference->getAttachedMobileActor();
		if (mact == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
		}

		if (!mact->getIsWerewolf()) {
			return false;
		}

		if (mact->isDead()) {
			return false;
		}

		mact->changeWerewolfState(false);
		mact->setMobileActorFlag(TES3::MobileActorFlag::BodypartsChanged, true);

		auto macp = TES3::WorldController::get()->getMobilePlayer();
		if (mact == macp) {
			auto worldController = TES3::WorldController::get();

			// Change FOV.
			worldController->shadowCamera.cameraData.setFOV(75.0f);
			worldController->worldCamera.cameraData.setFOV(75.0f);

			// Update faders.
			worldController->sunglareFader->activate();
			worldController->werewolfFader->deactivate();
		}

		return true;
	}

	void setMarkLocation(sol::table params) {
		auto macp = TES3::WorldController::get()->getMobilePlayer();

		// Get the position.
		sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
		if (!position) {
			throw std::invalid_argument("Invalid 'position' parameter provided.");
		}

		// Get the orientation.
		float rotation = getOptionalParam<float>(params, "rotation", macp->reference->orientation.z);

		// Get the cell.
		TES3::Cell* cell = getOptionalParamCell(params, "cell");
		if (cell == nullptr) {
			// Try to find an exterior cell from the position.
			int gridX = TES3::Cell::toGridCoord(position.value().x);
			int gridY = TES3::Cell::toGridCoord(position.value().y);
			cell = TES3::DataHandler::get()->nonDynamicData->getCellByGrid(gridX, gridY);
			if (cell == nullptr) {
				throw std::invalid_argument("Invalid 'cell' parameter provided. Could not resolve cell by exterior position.");
			}
		}

		if (macp->markLocation == nullptr) {
			macp->markLocation = tes3::_new<TES3::MarkData>();
		}

		macp->markLocation->position = position.value();
		macp->markLocation->rotation = rotation;
		macp->markLocation->cell = cell;
	}

	void clearMarkLocation() {
		auto macp = TES3::WorldController::get()->getMobilePlayer();
		if (macp->markLocation) {
			tes3::_delete(macp->markLocation);
			macp->markLocation = nullptr;
		}
	}

	int getItemCount(sol::table params) {
		TES3::Reference* reference = getOptionalParamExecutionReference(params);
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}
		else if (!reference->baseObject->isActor()) {
			throw std::invalid_argument("Invalid 'reference' parameter provided: reference does not have an inventory.");
		}

		auto item = getOptionalParamObject<TES3::Item>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}

		auto asActor = reinterpret_cast<TES3::Actor*>(reference->baseObject);
		auto stack = asActor->inventory.findItemStack(item);
		if (stack == nullptr) {
			return 0;
		}

		return std::abs(stack->count);
	}

	std::tuple<bool, sol::table> getItemIsStolen(sol::table params) {
		auto item = getOptionalParamObject<TES3::Item>(params, "item");

		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}
		else if (item->getStolenList() == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided. Does not support a stolen list.");
		}

		auto from = getOptionalParamObject<TES3::BaseObject>(params, "from");
		if (from) {
			return { item->getStolenFlag(from), item->getStolenList_lua(params.lua_state()) };
		}
		else {
			auto stolenList = item->getStolenList_lua(params.lua_state());
			return { !stolenList.empty(), stolenList };
		}
	}

	void setItemIsStolen(sol::table params) {
		auto item = getOptionalParamObject<TES3::Item>(params, "item");

		if (item == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided.");
		}
		else if (item->getStolenList() == nullptr) {
			throw std::invalid_argument("Invalid 'item' parameter provided. Does not support a stolen list.");
		}

		auto from = getOptionalParamObject<TES3::BaseObject>(params, "from");
		const auto stolen = getOptionalParam<bool>(params, "stolen", true);
		if (from) {
			if (getOptionalParam<bool>(params, "stolen", true)) {
				item->addStolenFlag(from);
			}
			else {
				item->removeStolenFlag(from);
			}
		}
		else {
			if (!stolen) {
				item->getStolenList()->clear();
			}
			else {
				throw std::invalid_argument("Invalid 'from' parameter provided. An item cannot be set as generically stolen.");
			}
		}

	}

	// Legacy tes3.getOwner.
	TES3::BaseObject* getOwnerLegacy(TES3::Reference* reference) {
		auto itemData = reference->getAttachedItemData();
		if (itemData == nullptr) {
			return nullptr;
		}

		return itemData->owner;
	}

	std::tuple<sol::object, sol::object> getOwner(sol::table params) {
		// Get the reference to get ownership for.
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		auto itemData = reference->getAttachedItemData();
		if (itemData == nullptr || itemData->owner == nullptr) {
			return {};
		}

		if (itemData->owner->objectType == TES3::ObjectType::NPC) {
			return std::make_tuple<sol::object, sol::object>(itemData->owner->getOrCreateLuaObject(params.lua_state()), itemData->requiredVariable->getOrCreateLuaObject(params.lua_state()));
		}
		else if (itemData->owner->objectType == TES3::ObjectType::Faction) {
			return std::make_tuple<sol::object, sol::object>(itemData->owner->getOrCreateLuaObject(params.lua_state()), sol::make_object(params.lua_state(), itemData->requiredRank));
		}
		else {
			throw std::runtime_error("Owner was not of a valid type. Report this issue to MWSE developers!");
		}
	}

	void setOwner(sol::table params) {
		// Get the reference to set ownership for.
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		if (reference == nullptr) {
			throw std::invalid_argument("Invalid 'reference' parameter provided.");
		}

		// Are we removing an actor?
		if (getOptionalParam<bool>(params, "remove", false)) {
			auto itemData = reference->getAttachedItemData();
			if (itemData) {
				itemData->owner = nullptr;
				itemData->requiredVariable = nullptr;
			}
			return;
		}

		// Are we getting an actor?
		TES3::BaseObject* owner = getOptionalParamBaseObject(params, "owner");
		if (owner == nullptr) {
			throw std::invalid_argument("Invalid 'owner' parameter provided.");
		}

		// Setting an NPC?
		else if (owner->objectType == TES3::ObjectType::NPC) {
			auto itemData = reference->getOrCreateAttachedItemData();
			itemData->owner = owner;
			itemData->requiredVariable = getOptionalParamObject<TES3::GlobalVariable>(params, "requiredGlobal");
		}

		// Setting a faction?
		else if (owner->objectType == TES3::ObjectType::Faction) {
			auto itemData = reference->getOrCreateAttachedItemData();
			itemData->owner = owner;
			itemData->requiredRank = getOptionalParam(params, "requiredRank", 0);
		}

		// Given a bogus object type?
		else {
			throw std::invalid_argument("Invalid 'owner' parameter provided: must be of type NPC or Faction.");
		}

		// Finish up.
		reference->setObjectModified(true);
	}

	int getKillCount(sol::optional<sol::table> params) {
		if (params) {
			auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
			if (actor == nullptr) {
				throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
			}
			return TES3::WorldController::get()->playerKills->getKillCount(actor);
		}
		else {
			return TES3::WorldController::get()->playerKills->totalKills;
		}
	}

	void setKillCount(sol::table params) {
		auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
		if (actor == nullptr) {
			throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
		}

		sol::optional<int> count = params["count"];
		if (!count) {
			throw std::invalid_argument("Invalid 'count' parameter provided: must be a number.");
		}

		TES3::WorldController::get()->playerKills->setKillCount(actor, count.value());
	}

	void decrementKillCount(sol::table params) {
		auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
		if (actor == nullptr) {
			throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
		}
		TES3::WorldController::get()->playerKills->decrement(actor);
	}

	void incrementKillCount(sol::table params) {
		auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
		if (actor == nullptr) {
			throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
		}
		TES3::WorldController::get()->playerKills->increment(actor);
	}

	sol::table getKillCounts(sol::this_state ts) {
		sol::state_view state = ts;
		sol::table killMap = state.create_table();
		for (auto& itt : *TES3::WorldController::get()->playerKills->counter) {
			killMap[itt.first] = itt.second;
		}
		return killMap;
	}

	int getWerewolfKillCount() {
		return TES3::WorldController::get()->playerKills->werewolfKills;
	}

	void setWerewolfKillCount(int count) {
		TES3::WorldController::get()->playerKills->werewolfKills = count;
	}

	TES3::Birthsign* findBirthsign(const char* id) {
		for (auto birthsign : *TES3::DataHandler::get()->nonDynamicData->birthsigns) {
			if (mwse::string::iequal(id, birthsign->id)) {
				return birthsign;
			}
		}
		return nullptr;
	}

	void setSourceless(TES3::BaseObject* object, sol::optional<bool> sourceless) {
		if (object) {
			object->setSourceless(sourceless.value_or(true));
		}
	}

	bool canRest_internal(sol::optional<sol::table> params, bool showMessageByDefault) {
		auto dataHandler = TES3::DataHandler::get();
		auto worldController = TES3::WorldController::get();
		if (!worldController || !dataHandler) return false;

		// Check to see if there are enemies nearby.
		if (getOptionalParam<bool>(params, "checkForEnemies", true) && !worldController->mobManager->processManager->checkNearbyEnemiesAllowRest()) {
			if (getOptionalParam<bool>(params, "showMessage", showMessageByDefault)) {
				TES3::UI::showMessageBox(dataHandler->nonDynamicData->GMSTs[TES3::GMST::sNotifyMessage2]->value.asString, nullptr, true);
			}
			return false;
		}

		// Check to make sure the player isn't underwater, jumping, or flying.
		auto macp = worldController->getMobilePlayer();
		if (getOptionalParam<bool>(params, "checkForSolidGround", true)) {
			const auto excludedMovement = TES3::ActorMovement::Flying | TES3::ActorMovement::Jumping;
			if ((macp->movementFlags & excludedMovement) || macp->getBasePositionIsUnderwater()) {
				if (getOptionalParam<bool>(params, "showMessage", showMessageByDefault)) {
					TES3::UI::showMessageBox(dataHandler->nonDynamicData->GMSTs[TES3::GMST::sNotifyMessage1]->value.asString, nullptr, true);
				}
				return false;
			}
		}

		return true;
	}

	sol::optional<bool> canRest(sol::optional<sol::table> params) {
		auto dataHandler = TES3::DataHandler::get();
		auto worldController = TES3::WorldController::get();
		if (!worldController || !dataHandler) return {};

		return canRest_internal(params, false);
	}

	bool showRestMenu(sol::optional<sol::table> params) {
		auto worldController = TES3::WorldController::get();
		auto dataHandler = TES3::DataHandler::get();
		if (!worldController || !dataHandler) return false;

		// Perform standard rest checks.
		if (!canRest_internal(params, true)) {
			return false;
		}

		// Allow using resting/waiting param. Defaults to resting.
		bool resting = getOptionalParam(params, "resting", !getOptionalParam(params, "waiting", false));

		// Is sleeping illegal here?
		if (resting && getOptionalParam<bool>(params, "checkSleepingIllegal", true) && dataHandler->currentCell->getSleepingIsIllegal()) {
			resting = false;
		}

		// Werewolves can't sleep.
		auto macp = worldController->getMobilePlayer();
		if (resting && getOptionalParam<bool>(params, "checkIsWerewolf", true) && macp->getIsWerewolf()) {
			resting = false;
		}

		// Finally show the message.
		TES3::UI::showRestMenu(resting);
		return true;
	}

	const auto TES3_UI_closeRestMenu = reinterpret_cast<char(__cdecl*)()>(0x610420);
	void closeRestMenu() {
		TES3::UI::Element* menuRest = TES3::UI::findMenu("MenuRestWait");
		if (menuRest) {
			TES3_UI_closeRestMenu();
		}
	}

	bool showSpellmakingMenu(sol::optional<sol::table> params) {
		auto serviceActor = getOptionalParamMobileActor(params, "serviceActor");
		if (serviceActor == nullptr && getOptionalParam(params, "useDialogActor", true)) {
			serviceActor = TES3::UI::getServiceActor();
			if (serviceActor == nullptr) {
				throw std::runtime_error("No dialog actor can be resolved. Provide a service actor or set useDialogActor to false.");
			}
		}
		TES3::UI::showSpellmakingMenuWithOverride(serviceActor);
		TES3::UI::enterMenuMode("MenuSpellmaking");
		return true;
	}

	const auto TES3_UI_closeSpellmakingMenu = reinterpret_cast<char(__cdecl*)(TES3::UI::Element*)>(0x621C60);
	const auto TES3_UI_closeMenuSetValues = reinterpret_cast<char(__cdecl*)(TES3::UI::Element*)>(0x61B870);
	void closeSpellmakingMenu() {
		TES3::UI::Element* menuSpellmaking = TES3::UI::findMenu("MenuSpellmaking");
		if (menuSpellmaking) {
			TES3::UI::Element* menuSetValues = TES3::UI::findMenu("MenuSetValues");
			if (menuSetValues) {
				TES3::UI::Element* menuSetValuesCancelButton = menuSetValues->findChild("MenuSetValues_Cancelbutton");
				TES3_UI_closeMenuSetValues(menuSetValuesCancelButton);
			}
			TES3::UI::Element* menuSpellmakingCancelButton = menuSpellmaking->findChild("MenuSpellmaking_Cancelbutton");
			TES3_UI_closeSpellmakingMenu(menuSpellmakingCancelButton);
		}
	}

	bool setPlayerControlState(sol::optional<sol::table> params) {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			return false;
		}

		auto macp = worldController->getMobilePlayer();
		if (!macp) {
			return false;
		}

		// Set individual control states.
		auto enabled = getOptionalParam<bool>(params, "enabled", false);
		macp->controlsDisabled = !enabled;
		macp->attackDisabled = !getOptionalParam(params, "attack", enabled);
		macp->jumpingDisabled = !getOptionalParam(params, "jumping", enabled);
		macp->magicDisabled = !getOptionalParam(params, "magic", enabled);
		macp->vanityDisabled = !getOptionalParam(params, "vanity", enabled);
		macp->viewSwitchDisabled = !getOptionalParam(params, "viewSwitch", enabled);
		return true;
	}

	bool wakeUp() {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			return false;
		}

		auto macp = worldController->getMobilePlayer();
		if (!macp) {
			return false;
		}

		// Make sure we're currently sleeping/waiting.
		if (!macp->sleeping && !macp->waiting) {
			return false;
		}

		macp->wakeUp();
		return true;
	}

	std::tuple<unsigned int, unsigned int> getViewportSize() {
		auto game = TES3::Game::get();
		return { game->windowWidth, game->windowHeight };
	}

	bool setVanityMode(sol::optional<sol::table> params) {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			return false;
		}

		auto macp = worldController->getMobilePlayer();
		if (!macp) {
			return false;
		}

		// Do nothing if vanity is disabled.
		if (macp->vanityDisabled && getOptionalParam(params, "checkVanityDisabled", true)) {
			return false;
		}

		// Allow toggling.
		bool enabled = getOptionalParam(params, "enabled", true);
		if (getOptionalParam(params, "toggle", false)) {
			enabled = macp->getVanityState() == 0;
		}

		macp->setVanityState(enabled ? 2 : 0);
		return true;
	}

	bool getVanityMode() {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			return false;
		}

		auto macp = worldController->getMobilePlayer();
		if (!macp) {
			return false;
		}

		return macp->getVanityState() != 0;
	}

	bool testLineOfSight(sol::table params) {
		// Were we given position data directly?
		auto position1 = getOptionalParamVector3(params, "position1");
		if (position1) {
			auto position2 = getOptionalParamVector3(params, "position2");
			auto height1 = getOptionalParam<float>(params, "height1", 0.0f);
			auto height2 = getOptionalParam<float>(params, "height2", 0.0f);

			if (!position2) {
				throw std::invalid_argument("Invalid positional params provided. Must provided two references or two positions.");
			}

			return tes3::testLineOfSight(&position1.value(), height1, &position2.value(), height2);
		}

		// Were we given two actors?
		auto reference1 = getOptionalParamReference(params, "reference1");
		auto reference2 = getOptionalParamReference(params, "reference2");
		if (!reference1 || !reference2) {
			throw std::invalid_argument("Invalid positional params provided. Must provided two references or two positions/heights.");
		}

		//
		sol::optional<TES3::Vector3> position2;
		float height1, height2;

		// Try to get the first reference's data.
		auto mobile1 = reference1->getAttachedMobileObject();
		position1 = reference1->position;
		if (mobile1) {
			height1 = mobile1->height;
		}
		else if (reference1->baseObject->boundingBox) {
			// Use distance from centre to top of bounding box as height.
			height1 = reference1->baseObject->boundingBox->maximum.z;
		}
		else {
			// Possibly the asset or its collision is uninitialized. Assume height is zero instead of failing.
			height1 = 0;
		}

		// Try to get the second reference's data.
		auto mobile2 = reference2->getAttachedMobileObject();
		position2 = reference2->position;
		if (mobile2) {
			height2 = mobile2->height;
		}
		else if (reference2->baseObject->boundingBox) {
			// Use distance from centre to top of bounding box as height.
			height2 = reference2->baseObject->boundingBox->maximum.z;
		}
		else {
			// Possibly the asset or its collision is uninitialized. Assume height is zero instead of failing.
			height2 = 0;
		}

		return tes3::testLineOfSight(&position1.value(), height1, &position2.value(), height2);
	}

	sol::object findActorsInProximity(sol::table params) {
		TES3::Reference* reference = getOptionalParamReference(params, "reference");
		auto position = getOptionalParamVector3(params, "position");
		auto range = getOptionalParam<float>(params, "range");

		if (reference) {
			position = reference->position;

			// Use centre of body location to match how the findActorsInProximity treats mobiles.
			auto mobile = reference->getAttachedMobileObject();
			if (mobile) {
				position.value().z += 0.5f * mobile->height;
			}
		}

		if (!position) {
			throw std::invalid_argument("A valid 'reference' or 'position' parameter is required.");
		}
		if (!range) {
			throw std::invalid_argument("Invalid 'range' parameter provided.");
		}

		TES3::IteratedList<TES3::MobileActor*> actors;
		auto processManager = TES3::WorldController::get()->mobManager->processManager;
		processManager->findActorsInProximity(&position.value(), range.value(), &actors);

		// Convert list to lua array.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		sol::table result = stateHandle.state.create_table();
		for (auto& i : actors) {
			result.add(i);
		}

		return result;
	}

	int calculatePrice(sol::table params) {
		bool buying = getOptionalParam(params, "buying", !getOptionalParam(params, "selling", false));
		bool isBartering = getOptionalParam(params, "bartering", false);
		bool isRepairing = getOptionalParam(params, "repairing", false);
		bool isTraining = getOptionalParam(params, "training", false);

		// Get the base value.
		auto object = getOptionalParamObject<TES3::Object>(params, "object");
		auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
		auto basePrice = getOptionalParam(params, "basePrice", object ? object->getValue() : 0);
		if (object && object->objectType == TES3::ObjectType::Spell) {
			basePrice = static_cast<TES3::Spell*>(object)->getValue();
		}
		if (isRepairing) {
			// Standard repair price calculation. Note it uses integer divisions.
			if (object && itemData) {
				auto fRepairMult = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fRepairMult];
				auto maxCondition = object->getDurability();
				int repairRatio = std::max(1, maxCondition / std::max(1, basePrice));
				int x = (maxCondition - itemData->condition) / repairRatio;
				basePrice = int(fRepairMult->value.asFloat * x);
			}
			else {
				throw std::invalid_argument("Invalid 'object' or 'itemData' parameter provided.");
			}
		}
		else if (isTraining) {
			// Standard training price calculation.
			auto skill = getOptionalParam(params, "skill", TES3::SkillID::Invalid);
			if (skill >= TES3::SkillID::FirstSkill && skill <= TES3::SkillID::LastSkill) {
				auto iTrainingMod = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::iTrainingMod];
				auto& skillAttr = TES3::WorldController::get()->getMobilePlayer()->skills[skill];
				basePrice = int(iTrainingMod->value.asLong * skillAttr.base);
			}
			else {
				throw std::invalid_argument("Invalid 'skill' parameter provided.");
			}
		}

		// Get the first-modified value.
		auto merchant = getOptionalParamMobileActor(params, "merchant");
		if (merchant == nullptr) {
			throw std::invalid_argument("Invalid 'merchant' parameter provided.");
		}
		auto price = merchant->determineModifiedPrice(basePrice, buying);

		// Fire off the appropriate event.
		event::BaseEvent* firedEvent = nullptr;
		if (isBartering) {
			if (event::CalculateSpellPriceEvent::getEventEnabled() && object && object->objectType == TES3::ObjectType::Spell) {
				firedEvent = new event::CalculateSpellPriceEvent(merchant, basePrice, price, static_cast<TES3::Spell*>(object));
			}
			else if (event::CalculateBarterPriceEvent::getEventEnabled()) {
				auto count = getOptionalParam(params, "count", 1);
				firedEvent = new event::CalculateBarterPriceEvent(merchant, basePrice, price, buying, count, object, itemData);
			}
		}
		else if (isRepairing) {
			if (event::CalculateRepairPriceEvent::getEventEnabled()) {
				firedEvent = new event::CalculateRepairPriceEvent(merchant, basePrice, price, object, itemData);
			}
		}
		else if (isTraining) {
			if (event::CalculateTrainingPriceEvent::getEventEnabled()) {
				auto skill = getOptionalParam(params, "skill", TES3::SkillID::Invalid);
				firedEvent = new event::CalculateTrainingPriceEvent(merchant, basePrice, price, skill);
			}
		}

		// Allow the event to modify things, if we have one.
		if (firedEvent) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table result = stateHandle.triggerEvent(firedEvent);
			if (result.valid()) {
				price = result.get_or("price", price);
			}
		}

		return price;
	}

	int calculateChargeUse(sol::table params) {
		auto enchant = getOptionalParam<TES3::Enchantment*>(params, "enchantment").value_or(nullptr);
		auto mobile = getOptionalParamMobileActor(params, "mobile");

		if (!enchant) {
			throw std::invalid_argument("calculateChargeUse: enchantment parameter required.");
		}
		if (!mobile) {
			throw std::invalid_argument("calculateChargeUse: mobile parameter required.");
		}

		// Cast once items do not use charges.
		if (enchant->castType == TES3::EnchantmentCastType::Once) {
			return 0;
		}

		// Calculate base charge cost.
		int charge = enchant->chargeCost;
		int skill = mobile->getSkillValue(TES3::SkillID::Enchant);

		// Check for enchantedItemRebalance patch to select correct charge calculation.
		if (mcp::getFeatureEnabled(mcp::feature::EnchantedItemRebalance)) {
			charge = int(charge * (250.0 - skill) * 0.004);
		}
		else {
			charge = int(charge * (110.0 - skill) * 0.01);
		}
		charge = std::max(1, charge);

		// Fire off the event.
		if (event::EnchantChargeUseEvent::getEventEnabled()) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();

			sol::object eventResult = stateHandle.triggerEvent(new event::EnchantChargeUseEvent(enchant, mobile, nullptr, charge));

			// Allow the event to modify charge.
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				sol::optional<float> newCharge = eventData["charge"];
				if (newCharge) {
					charge = newCharge.value();
				}
			}
		}

		return charge;
	}

	TES3::VFX* createVisualEffect(sol::table params) {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			throw std::runtime_error("This function cannot be called before the world controller is created.");
		}

		auto vfxManager = worldController->vfxManager;
		if (!vfxManager) {
			throw std::runtime_error("This function cannot be called before the VFX manager is created.");
		}

		// Legacy support for old param names.
		auto effectObject = getOptionalParamObject<TES3::PhysicalObject>(params, "object");
		if (!effectObject) {
			effectObject = getOptionalParamObject<TES3::PhysicalObject>(params, "effect");
		}

		if (effectObject) {
			auto serial = getOptionalParam(params, "serial", 0u);
			auto reference = getOptionalParamReference(params, "reference");
			auto repeatCount = getOptionalParam(params, "repeatCount", 0);
			auto lifespan = getOptionalParam(params, "lifespan", TES3::VFX::AGE_INFINITE);
			auto scale = getOptionalParam(params, "scale", 1.0f);
			auto verticalOffset = getOptionalParam(params, "verticalOffset", 0.0f);

			// First see if we want to make it at a position.
			auto position = getOptionalParamVector3(params, "position");
			if (position) {
				return vfxManager->createAtPosition(serial, effectObject, &position.value(), repeatCount, lifespan, scale, verticalOffset);
			}

			// Then see if we want to attach it to an AVObject.
			auto avObject = getOptionalParam<NI::AVObject*>(params, "avObject");
			if (avObject) {
				return vfxManager->createForAVObject(serial, effectObject, avObject.value(), repeatCount, lifespan, scale, verticalOffset);
			}

			return vfxManager->createForReference(serial, effectObject, reference, repeatCount, lifespan, scale, verticalOffset);
		}

		// Legacy support for old param names.
		auto enchantEffect = getOptionalParam<int>(params, "magicEffectId");
		if (!enchantEffect) {
			enchantEffect = getOptionalParam<int>(params, "enchantment");
		}

		if (enchantEffect) {
			if (!TES3::DataHandler::get()->nonDynamicData->getMagicEffect(enchantEffect.value())) {
				throw std::invalid_argument("Invalid 'magicEffectId' parameter. Effect of the given index does not exist.");
			}
			auto reference = getOptionalParamReference(params, "reference");
			auto lifespan = getOptionalParam(params, "lifespan", TES3::VFX::AGE_INFINITE);
			return vfxManager->createForMagicEffect(enchantEffect.value(), reference, lifespan);
		}

		throw std::invalid_argument("No valid parameters defined. No operation performed.");
	}

	int removeVisualEffect(sol::table params) {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			throw std::runtime_error("This function cannot be called before the world controller is created.");
		}

		auto vfxManager = worldController->vfxManager;
		if (!vfxManager) {
			throw std::runtime_error("This function cannot be called before the VFX manager is created.");
		}

		const auto countBefore = vfxManager->vfxNodes.size();

		auto vfx = getOptionalParam<TES3::VFX*>(params, "vfx", nullptr);
		auto avObject = getOptionalParam<NI::AVObject*>(params, "avObject", nullptr);
		auto reference = getOptionalParamReference(params, "reference");
		auto serial = getOptionalParam<unsigned int>(params, "serial");

		if (vfx) {
			vfxManager->remove(vfx);
		}

		else if (avObject) {
			vfxManager->removeFromAVObject(avObject);
		}

		else if (serial && reference) {
			vfxManager->removeForSerialForReference(serial.value(), reference);
		}

		else if (reference) {
			vfxManager->removeForReference(reference);
		}
		else {
			throw std::invalid_argument("No valid parameters defined. No operation performed.");
		}

		return countBefore - vfxManager->vfxNodes.size();
	}

	const char* applyTextDefines(sol::table params) {
		auto worldController = TES3::WorldController::get();
		if (!worldController) {
			throw std::runtime_error("This function cannot be called before the world controller is created.");
		}

		if (!worldController->getMobilePlayer()) {
			throw std::runtime_error("This function cannot be called before the player is created.");
		}

		auto text = getOptionalParam<const char*>(params, "text", nullptr);
		if (!text) {
			throw std::invalid_argument("Invalid 'text' parameter provided.");
		}

		const auto actor = static_cast<TES3::Actor*>(getOptionalParamBaseObject(params, "actor"));
		if (!actor->isActor()) {
			throw std::invalid_argument("Invalid 'actor' parameter provided.");
		}

		worldController->fonts[0]->substituteTextMacros(actor, text);
		return worldController->fonts[0]->getSubstituteResult();
	}

	bool addTopic(sol::optional<sol::table> params) {
		const auto worldController = TES3::WorldController::get();
		if (!worldController) {
			throw std::runtime_error("This function cannot be called before the world controller is initialized.");
		}

		const auto macp = worldController->getMobilePlayer();
		if (!macp) {
			throw std::runtime_error("This function cannot be called before the player is initialized.");
		}

		const auto topic = getOptionalParamDialogue(params, "topic");
		if (!topic) {
			throw std::invalid_argument("Invalid 'topic' parameter provided.");
		}
		else if (topic->type != TES3::DialogueType::Topic) {
			throw std::invalid_argument("Invalid 'topic' parameter provided. Must be of the the correct dialogue type (topic).");
		}

		const auto topicCountBefore = macp->dialogueList->size();
		macp->addTopic(topic);

		if (getOptionalParam<bool>(params, "updateGUI", true)) {
			TES3::UI::updateTopicsList();
		}

		return macp->dialogueList->size() > topicCountBefore;
	}

	bool showDialogueMenu(sol::optional<sol::table> params) {
		const auto worldController = TES3::WorldController::get();
		if (!worldController) {
			throw std::runtime_error("This function cannot be called before the world controller is initialized.");
		}

		const auto macp = worldController->getMobilePlayer();
		if (!macp) {
			throw std::runtime_error("This function cannot be called before the player is initialized.");
		}

		auto target = getOptionalParamMobileActor(params, "reference");
		if (!target) {
			throw std::runtime_error("Invalid 'reference' parameter provided. The reference must have a valid mobile attached.");
		}

		// Make werewolf check.
		if (getOptionalParam<bool>(params, "checkAllowWerewolfForceGreeting", true)) {
			auto targetScript = target->reference->baseObject->getScript();
			if (targetScript && macp->getIsWerewolf()) {
				unsigned int variableIndex = -1;
				if (!targetScript->getLocalVarIndexAndType("AllowWerewolfForceGreeting", &variableIndex)) {
					// The actual variable value and type doesn't matter here.
					return false;
				}
			}
		}

		target->startDialogue();
		return TES3::UI::findMenu("MenuDialog") != nullptr;
	}

	const auto TES3_UI_goodbye = reinterpret_cast<bool(__cdecl*)()>(0x5BF7C0);
	bool closeDialogueMenu(sol::optional<sol::table> params) {
		auto menuDialog = TES3::UI::findMenu("MenuDialog");
		if (!menuDialog) {
			return false;
		}

		if (getOptionalParam<bool>(params, "force", true)) {
			// Destroy any dialogue choices, which goodbye also checks for.
			while (auto answerBlock = menuDialog->findChild("MenuDialog_answer_block")) {
				answerBlock->destroy();
			}

			// This global controls if we are in an answering mode, and blocks things like the usual goodbye button.
			*reinterpret_cast<bool*>(0x7D3568) = 0;
		}

		TES3_UI_goodbye();

		return TES3::UI::findMenu("MenuDialog") == nullptr;
	}

	const auto TES3_UI_showContentsMenu = reinterpret_cast<void(__cdecl*)(TES3::Actor*, TES3::Reference*, TES3::MobileActor*, bool)>(0x5B4220);
	const auto TES3_UI_closeContentsMenu = reinterpret_cast<void(__cdecl*)()>(0x5B7330);
	bool showContentsMenu(sol::optional<sol::table> params) {
		if (TES3::UI::findMenu("MenuContents")) {
			TES3_UI_closeContentsMenu();
			if (TES3::UI::findMenu("MenuContents")) {
				return false;
			}
		}

		auto reference = getOptionalParamReference(params, "reference");
		if (!reference) {
			throw std::runtime_error("Invalid 'reference' parameter provided.");
		}
		else if (!reference->baseObject->isActor()) {
			throw std::runtime_error("Invalid 'reference' parameter provided. It does not have an inventory.");
		}

		// Clone the reference if we need to.
		reference->clone();

		// Get pickpocket flag. The take all button gets funky if using it with NPCs with pickpocket turned off.
		auto pickpocket = getOptionalParam<bool>(params, "pickpocket", false);

		auto macp = TES3::WorldController::get()->getMobilePlayer();
		TES3_UI_showContentsMenu(static_cast<TES3::Actor*>(reference->baseObject), reference, macp, pickpocket);
		TES3::UI::enterMenuMode("MenuContents");

		return TES3::UI::findMenu("MenuContents") != nullptr;
	}

	bool closeContentsMenu(sol::optional<sol::table> params) {
		if (!TES3::UI::findMenu("MenuContents")) {
			return false;
		}

		TES3_UI_closeContentsMenu();

		return TES3::UI::findMenu("MenuContents") == nullptr;
	}

	bool payMerchant(sol::optional<sol::table> params) {
		auto mobile = getOptionalParamMobileActor(params, "merchant");
		if (!mobile) {
			throw std::runtime_error("Invalid 'merchant' parameter provided.");
		}
		auto costParam = getOptionalParam<int>(params, "cost");
		if (!costParam) {
			throw std::runtime_error("Invalid 'cost' parameter provided.");
		}

		auto worldController = TES3::WorldController::get();
		auto macp = worldController->getMobilePlayer();
		int playerGold = macp->getGold();
		int cost = costParam.value();

		bool success = false;
		if (cost > 0 && cost <= playerGold) {
			success = true;
		}
		else if (cost < 0 && (-cost) <= mobile->barterGold) {
			success = true;
		}

		if (success) {
			// Transfer gold.
			macp->modGold(-cost);
			mobile->barterGold += cost;

			// Extend refresh timeout for barterGold refresh system. This prevents the change from being overwritten immediately.
			auto hourStamp = worldController->gvarDaysPassed->value * 24 + worldController->gvarGameHour->value;
			if (mobile->actionData.lastBarterHoursPassed == 0) {
				mobile->actionData.lastBarterHoursPassed = hourStamp;
			}
			else {
				auto resetDelay = int(TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fBarterGoldResetDelay]->value.asFloat);
				if (hourStamp >= mobile->actionData.lastBarterHoursPassed + resetDelay) {
					mobile->actionData.lastBarterHoursPassed = hourStamp;
				}
			}
		}
		return success;
	}

	void bindTES3Util() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		//
		// Extend tes3 library with extra functions.
		//

		sol::table tes3 = state["tes3"];

		tes3["addArmorSlot"] = addArmorSlot;
		tes3["addClothingSlot"] = addClothingSlot;
		tes3["addItem"] = addItem;
		tes3["addItemData"] = addItemData;
		tes3["addJournalEntry"] = addJournalEntry;
		tes3["addMagicEffect"] = addMagicEffect;
		tes3["addSoulGem"] = addSoulGem;
		tes3["addSpell"] = addSpell;
		tes3["addTopic"] = addTopic;
		tes3["adjustSoundVolume"] = adjustSoundVolume;
		tes3["advanceTime"] = advanceTime;
		tes3["applyConstantEffectEquipment"] = applyConstantEffectEquipment;
		tes3["applyMagicSource"] = applyMagicSource;
		tes3["applyTextDefines"] = applyTextDefines;
		tes3["beginTransform"] = beginTransform;
		tes3["calculateChargeUse"] = calculateChargeUse;
		tes3["calculatePrice"] = calculatePrice;
		tes3["cancelAnimationLoop"] = cancelAnimationLoop;
		tes3["canRest"] = canRest;
		tes3["cast"] = cast;
		tes3["changeWeather"] = changeWeather;
		tes3["checkMerchantOffersService"] = checkMerchantOffersService;
		tes3["checkMerchantTradesItem"] = checkMerchantTradesItem;
		tes3["clearMarkLocation"] = clearMarkLocation;
		tes3["closeAlchemyMenu"] = closeAlchemyMenu;
		tes3["closeContentsMenu"] = closeContentsMenu;
		tes3["closeDialogueMenu"] = closeDialogueMenu;
		tes3["closeRepairServiceMenu"] = closeRepairServiceMenu;
		tes3["closeRestMenu"] = closeRestMenu;
		tes3["closeSpellmakingMenu"] = closeSpellmakingMenu;
		tes3["createCell"] = createCell;
		tes3["createObject"] = createObject;
		tes3["createReference"] = createReference;
		tes3["createVisualEffect"] = createVisualEffect;
		tes3["decrementKillCount"] = decrementKillCount;
		tes3["deleteObject"] = deleteObject;
		tes3["disableKey"] = disableKey;
		tes3["dropItem"] = dropItem;
		tes3["enableKey"] = enableKey;
		tes3["equip"] = equip;
		tes3["fadeIn"] = fadeIn;
		tes3["fadeOut"] = fadeOut;
		tes3["fadeTo"] = fadeTo;
		tes3["findActorsInProximity"] = findActorsInProximity;
		tes3["findBirthsign"] = findBirthsign;
		tes3["findClass"] = findClass;
		tes3["findClosestExteriorReferenceOfObject"] = findClosestExteriorReferenceOfObject;
		tes3["findDialogue"] = findDialogue;
		tes3["findGlobal"] = findGlobal;
		tes3["findGMST"] = findGMST;
		tes3["findQuest"] = findQuest;
		tes3["findRace"] = findRace;
		tes3["findRegion"] = findRegion;
		tes3["force1stPerson"] = force1stPerson;
		tes3["force3rdPerson"] = force3rdPerson;
		tes3["get3rdPersonCameraOffset"] = get3rdPersonCameraOffset;
		tes3["getActiveCells"] = getActiveCells;
		tes3["getAnimationActionTiming"] = getAnimationActionTiming;
		tes3["getAnimationGroups"] = getCurrentAnimationGroups;
		tes3["getAnimationTiming"] = getAnimationTiming;
		tes3["getArchiveList"] = getArchiveList;
		tes3["getCamera"] = getCamera;
		tes3["getCameraPosition"] = getCameraPosition;
		tes3["getCameraVector"] = getCameraVector;
		tes3["getCell"] = getCell;
		tes3["getClosestExteriorPosition"] = getClosestExteriorPosition;
		tes3["getCumulativeDaysForMonth"] = getCumulativeDaysForMonth;
		tes3["getCurrentAIPackageId"] = getCurrentAIPackageId;
		tes3["getCurrentWeather"] = getCurrentWeather;
		tes3["getCursorPosition"] = getCursorPosition;
		tes3["getDataHandler"] = TES3::DataHandler::get;
		tes3["getDaysInMonth"] = getDaysInMonth;
		tes3["getDialogueInfo"] = getDialogueInfo;
		tes3["getEffectMagnitude"] = getEffectMagnitude;
		tes3["getEquippedItem"] = getEquippedItem;
		tes3["getFaction"] = getFaction;
		tes3["getFileExists"] = getFileExists;
		tes3["getFileSource"] = getFileSource;
		tes3["getGame"] = TES3::Game::get;
		tes3["getGlobal"] = getGlobal;
		tes3["getGMST"] = getGMST;
		tes3["getInputBinding"] = getInputBinding;
		tes3["getItemCount"] = getItemCount;
		tes3["getItemIsStolen"] = getItemIsStolen;
		tes3["getJournalIndex"] = getJournalIndex;
		tes3["getKillCount"] = getKillCount;
		tes3["getKillCounts"] = getKillCounts;
		tes3["getLanguage"] = getLanguage;
		tes3["getLanguageCode"] = getLanguageCode;
		tes3["getLastExteriorPosition"] = getLastExteriorPosition;
		tes3["getLegacyScriptRunning"] = getLegacyScriptRunning;
		tes3["getLocked"] = getLocked;
		tes3["getLockLevel"] = getLockLevel;
		tes3["getMagicEffect"] = getMagicEffect;
		tes3["getMagicEffectName"] = getMagicEffectName;
		tes3["getMagicSourceInstanceBySerial"] = getMagicSourceInstanceBySerial;
		tes3["getMobilePlayer"] = getMobilePlayer;
		tes3["getModList"] = getModList;
		tes3["getObject"] = getObject;
		tes3["getOwner"] = sol::overload(getOwnerLegacy, getOwner);
		tes3["getPlayerActivationDistance"] = getPlayerActivationDistance;
		tes3["getPlayerCell"] = getPlayerCell;
		tes3["getPlayerEyePosition"] = getPlayerEyePosition;
		tes3["getPlayerEyeVector"] = getPlayerEyeVector;
		tes3["getPlayerGold"] = getPlayerGold;
		tes3["getPlayerRef"] = getPlayerRef;
		tes3["getPlayerTarget"] = getPlayerTarget;
		tes3["getQuickKey"] = getQuickKey;
		tes3["getReference"] = getReference;
		tes3["getRegion"] = getRegion;
		tes3["getScript"] = getScript;
		tes3["getSimulationTimestamp"] = getSimulationTimestamp;
		tes3["getSkill"] = getSkill;
		tes3["getSound"] = getSound;
		tes3["getSoundGenerator"] = getSoundGenerator;
		tes3["getSoundPlaying"] = getSoundPlaying;
		tes3["getTopMenu"] = TES3::UI::getMenuOnTop;
		tes3["getTrap"] = getTrap;
		tes3["getValue"] = getValue;
		tes3["getVanityMode"] = getVanityMode;
		tes3["getViewportSize"] = getViewportSize;
		tes3["getWerewolfKillCount"] = getWerewolfKillCount;
		tes3["getWorldController"] = TES3::WorldController::get;
		tes3["hammerKey"] = hammerKey;
		tes3["hasCodePatchFeature"] = hasCodePatchFeature;
		tes3["hasOwnershipAccess"] = hasOwnershipAccess;
		tes3["hasSpell"] = hasSpell;
		tes3["incrementKillCount"] = incrementKillCount;
		tes3["is3rdPerson"] = isThirdPerson;
		tes3["isAffectedBy"] = isAffectedBy;
		tes3["isModActive"] = isModActive;
		tes3["iterateObjects"] = iterateObjects;
		tes3["loadAnimation"] = loadAnimation;
		tes3["loadGame"] = loadGame;
		tes3["loadMesh"] = loadMesh;
		tes3["loadSourceTexture"] = loadSourceTexture;
		tes3["lock"] = lock;
		tes3["messageBox"] = messageBox;
		tes3["modDisposition"] = modDisposition;
		tes3["modStatistic"] = modStatistic;
		tes3["newGame"] = newGame;
		tes3["payMerchant"] = payMerchant;
		tes3["persuade"] = persuade;
		tes3["playAnimation"] = playAnimation;
		tes3["playItemPickupSound"] = playItemPickupSound;
		tes3["playSound"] = playSound;
		tes3["playVoiceover"] = playVoiceover;
		tes3["positionCell"] = positionCell;
		tes3["pushKey"] = pushKey;
		tes3["random"] = random;
		tes3["rayTest"] = rayTest;
		tes3["releaseKey"] = releaseKey;
		tes3["removeEffects"] = removeEffects;
		tes3["removeItem"] = removeItem;
		tes3["removeItemData"] = removeItemData;
		tes3["removeSound"] = removeSound;
		tes3["removeSpell"] = removeSpell;
		tes3["removeVisualEffect"] = removeVisualEffect;
		tes3["runLegacyScript"] = runLegacyScript;
		tes3["saveGame"] = saveGame;
		tes3["say"] = say;
		tes3["set3rdPersonCameraOffset"] = set3rdPersonCameraOffset;
		tes3["setAIActivate"] = setAIActivate;
		tes3["setAIEscort"] = setAIEscort;
		tes3["setAIFollow"] = setAIFollow;
		tes3["setAITravel"] = setAITravel;
		tes3["setAIWander"] = setAIWander;
		tes3["setAnimationTiming"] = setAnimationTiming;
		tes3["setDestination"] = setDestination;
		tes3["setEnabled"] = setEnabled;
		tes3["setExpelled"] = setExpelled;
		tes3["setGlobal"] = setGlobal;
		tes3["setItemIsStolen"] = setItemIsStolen;
		tes3["setJournalIndex"] = setJournalIndex;
		tes3["setKillCount"] = setKillCount;
		tes3["setLockLevel"] = setLockLevel;
		tes3["setMarkLocation"] = setMarkLocation;
		tes3["setOwner"] = setOwner;
		tes3["setPlayerControlState"] = setPlayerControlState;
		tes3["setSourceless"] = setSourceless;
		tes3["setStatistic"] = setStatistic;
		tes3["setTrap"] = setTrap;
		tes3["setVanityMode"] = setVanityMode;
		tes3["setWerewolfKillCount"] = setWerewolfKillCount;
		tes3["showAlchemyMenu"] = showAlchemyMenu;
		tes3["showContentsMenu"] = showContentsMenu;
		tes3["showDialogueMenu"] = showDialogueMenu;
		tes3["showRepairServiceMenu"] = showRepairServiceMenu;
		tes3["showRestMenu"] = showRestMenu;
		tes3["showSpellmakingMenu"] = showSpellmakingMenu;
		tes3["skipAnimationFrame"] = skipAnimationFrame;
		tes3["skipToNextMusicTrack"] = skipToNextMusicTrack;
		tes3["stopLegacyScript"] = stopLegacyScript;
		tes3["streamMusic"] = streamMusic;
		tes3["tapKey"] = tapKey;
		tes3["testLineOfSight"] = testLineOfSight;
		tes3["togglePOV"] = togglePOV;
		tes3["transferItem"] = transferItem;
		tes3["triggerCrime"] = triggerCrime;
		tes3["undoTransform"] = undoTransform;
		tes3["unhammerKey"] = unhammerKey;
		tes3["unlock"] = unlock;
		tes3["updateInventoryGUI"] = updateInventoryGUI;
		tes3["updateJournal"] = updateJournal;
		tes3["updateMagicGUI"] = updateMagicGUI;
		tes3["wakeUp"] = wakeUp;
	}
}
