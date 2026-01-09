#include "TES3ActorAnimationController.h"
#include "TES3AnimationData.h"
#include "TES3DataHandler.h"

#include "NINode.h"
#include "NIKeyframeManager.h"

#include "LuaManager.h"
#include "MemoryUtil.h"

#include "LuaAnimationTriggerEvent.h"
#include "LuaPlayAnimationGroupEvent.h"

namespace TES3 {
	constexpr int VanillaSourceCount = 3;
	constexpr int BaseAnimSourceIndex = 2;
	constexpr int BodySectionCount = 3;
	constexpr float fixedPointSpeedScale = 256.0f;

	//
	// TES3::AnimationDataVanilla
	//

	NI::Sequence* AnimationDataVanilla::SequenceGroup::at(int section) {
		return (&lower)[section];
	}

	const auto TES3_AnimationData_ctor = reinterpret_cast<void(__thiscall*)(AnimationDataVanilla*)>(0x46B7A0);
	AnimationDataVanilla* AnimationDataVanilla::ctor() {
		// Call original function.
		TES3_AnimationData_ctor(this);

		// Initialize custom data.
		patchedCastSpeed = (unsigned short)(1 * fixedPointSpeedScale);

		return this;
	}

	const auto TES3_AnimationData_playAnimationGroupForSection = reinterpret_cast<void(__thiscall*)(AnimationDataVanilla*, AnimGroupID, int, int, int)>(0x470AE0);
	void AnimationDataVanilla::playAnimationGroupForSection(AnimGroupID animationGroup, int bodySection, int startFlag, int loopCount) {
		if (mwse::lua::event::PlayAnimationGroupEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new mwse::lua::event::PlayAnimationGroupEvent(reinterpret_cast<AnimationData*>(this), int(animationGroup), bodySection, startFlag, loopCount));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;

				// Allow event blocking.
				if (eventData.get_or("block", false)) {
					return;
				}

				// Allow overrides.
				animationGroup = eventData.get_or("group", animationGroup);
				startFlag = eventData.get_or("flags", startFlag);
				loopCount = eventData.get_or("loopCount", loopCount);
			}
		}

		TES3_AnimationData_playAnimationGroupForSection(this, animationGroup, bodySection, startFlag, loopCount);
	}

	const auto TES3_AnimationData_setHeadNode = reinterpret_cast<void(__thiscall*)(AnimationDataVanilla*, NI::Node*)>(0x4704B0);
	void AnimationDataVanilla::setHeadNode(NI::Node* head) {
		TES3_AnimationData_setHeadNode(this, head);
	}

	const auto TES3_AnimationData_setSourceKeyframes = reinterpret_cast<bool(__thiscall*)(AnimationDataVanilla*, KeyframeDefinition*, int, bool)>(0x46BA30);
	bool AnimationDataVanilla::setSourceKeyframes(KeyframeDefinition* kfData, int sourceIndex, bool isBiped) {
		return TES3_AnimationData_setSourceKeyframes(this, kfData, sourceIndex, isBiped);
	}

	const auto TES3_AnimationData_updateMovementDelta = reinterpret_cast<bool(__thiscall*)(AnimationDataVanilla*, float, Vector3*, bool)>(0x470320);
	void AnimationDataVanilla::updateMovementDelta(float timing, Vector3 *inout_startingPosition, bool dontUpdatePositionDelta) {
		TES3_AnimationData_updateMovementDelta(this, timing, inout_startingPosition, dontUpdatePositionDelta);
	}

	const auto TES3_AnimationData_headTracking = reinterpret_cast<void(__thiscall*)(AnimationDataVanilla*, Reference*, Reference*)>(0x46F910);
	void AnimationDataVanilla::headTracking(Reference* actorRefr, Reference* targetRefr) {
		TES3_AnimationData_headTracking(this, actorRefr, targetRefr);
	}

	Reference* AnimationDataVanilla::getReference() const {
		if (actorNode) {
			return actorNode->getTes3Reference(false);
		}
		return nullptr;
	}

	void AnimationDataVanilla::playAnimationGroup(AnimGroupID animationGroup, int startFlag, int loopCount) {
		playAnimationGroupForSection(animationGroup, 0, startFlag, loopCount);
		playAnimationGroupForSection(animationGroup, 1, startFlag, loopCount);
		playAnimationGroupForSection(animationGroup, 2, startFlag, loopCount);
	}

	void AnimationDataVanilla::cancelAnimationLoop(bool jumpToLoopEnd) {
		// Cancel looping.
		for (int i = 0; i < BodySectionCount; ++i) {
			if (currentAnimGroups[i] != TES3::AnimGroupID::Idle) {
				loopCounts[i] = 0;
			}
		}

		if (jumpToLoopEnd) {
			// Advance the lower section animation directly to the end of the loop.
			auto lowerGroup = currentAnimGroups[0];
			if (AnimationGroup::getActionClass(lowerGroup) == AnimGroupActionClass::Looping) {
				const int Action_Looping_LoopEnd = 3;
				auto loopEndTiming = animationGroups[int(lowerGroup)]->actionTimings[Action_Looping_LoopEnd];

				// Only move timing forward. Apply to all sections with the same animation as the lower section.
				if (timings[0] < loopEndTiming) {
					for (int i = 0; i < BodySectionCount; ++i) {
						if (currentAnimGroups[i] == lowerGroup) {
							setTiming(lowerGroup, i, loopEndTiming);
						}
					}
				}
			}
		}
	}

	void AnimationDataVanilla::setTiming(AnimGroupID groupId, int bodySection, float t) {
		// Update timing.
		timings[bodySection] = t;

		// Update index of next soundgen event.
		int i, count = animGroupSoundGenCounts[int(groupId)];
		for (i = 0; i < count; ++i) {
			if (t <= animGroupSoundGens[int(groupId)][i]->startTime) {
				break;
			}
		}
		currentSoundGenIndices[bodySection] = i;
	}

	void AnimationData::swapAnimationGroups(AnimGroupID animationGroup1, AnimGroupID animationGroup2) {
		// Swap all animation group specific data.
		int g1 = int(animationGroup1), g2 = int(animationGroup2);
		std::swap(animationGroups[g1], animationGroups[g2]);
		std::swap(animGroupSourceIndices[g1], animGroupSourceIndices[g2]);
		std::swap(animGroupSoundGens[g1], animGroupSoundGens[g2]);
		std::swap(animGroupSoundGenCounts[g1], animGroupSoundGenCounts[g2]);
		std::swap(approxRootTravelSpeeds[g1], approxRootTravelSpeeds[g2]);

		// Fix up timing and sequence activation if the swap affects the currently playing animation.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto group = int(currentAnimGroups[i]);
			auto sequenceGroup = &this->customSources[i].lower;

			if (group == g1 || group == g2) {
				// Ensure correct sequence is activated.
				if (currentAnimGroupSources[i] != animGroupSourceIndices[group]) {
					manager->deactivateSequence(sequenceGroup[currentAnimGroupSources[i]]);
					manager->activateSequence(sequenceGroup[animGroupSourceIndices[group]]);
					currentAnimGroupSources[i] = animGroupSourceIndices[group];
				}

				// Reset timing to the start of the current action.
				float t = animationGroups[group]->actionTimings[currentActionIndices[i]];
				setTiming(AnimGroupID(group), i, t);
			}
		}

		// Fix up movement root if the swap affects the currently playing animation.
		auto lowerGroup = currentAnimGroups[0];
		if (lowerGroup == animationGroup1 || lowerGroup == animationGroup2) {
			Vector3 unused;
			updateMovementDelta(timings[0], &unused, true);
		}
	}

	float AnimationDataVanilla::getCastSpeed() const {
		return patchedCastSpeed / fixedPointSpeedScale;
	}

	void AnimationDataVanilla::setCastSpeed(float speed) {
		speed = std::max(0.0f, std::min(255.0f, speed));
		patchedCastSpeed = (unsigned short)(speed * fixedPointSpeedScale);

		// Update current animation speed if currently casting.
		if (currentAnimGroups[1] == AnimGroupID::SpellCast) {
			// Ensure non-zero weaponSpeed to bypass the actor controller resetting the value on zero.
			weaponSpeed = speed + std::numeric_limits<float>::min();
		}
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::currentAnimGroups)> AnimationDataVanilla::getCurrentAnimGroups() {
		return std::ref(currentAnimGroups);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::currentActionIndices)> AnimationDataVanilla::getCurrentActionIndices() {
		return std::ref(currentActionIndices);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::loopCounts)> AnimationDataVanilla::getLoopCounts() {
		return std::ref(loopCounts);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::timings)> AnimationDataVanilla::getTimings() {
		return std::ref(timings);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::animationGroups)> AnimationDataVanilla::getAnimationGroups() {
		return std::ref(animationGroups);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::keyframeSources)> AnimationDataVanilla::getKeyframeSources() {
		return std::ref(keyframeSources);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::currentAnimGroupSources)> AnimationDataVanilla::getCurrentAnimGroupSources() {
		return std::ref(currentAnimGroupSources);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::animGroupSourceIndices)> AnimationDataVanilla::getAnimGroupSourceIndices() {
		return std::ref(animGroupSourceIndices);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::approxRootTravelSpeeds)> AnimationDataVanilla::getApproxRootTravelSpeeds() {
		return std::ref(approxRootTravelSpeeds);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::currentSoundGenIndices)> AnimationDataVanilla::getCurrentSoundGenIndices() {
		return std::ref(currentSoundGenIndices);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::animGroupSoundGenCounts)> AnimationDataVanilla::getAnimGroupSoundGenCounts() {
		return std::ref(animGroupSoundGenCounts);
	}

	std::reference_wrapper<decltype(AnimationDataVanilla::animGroupSoundGens)> AnimationDataVanilla::getAnimGroupSoundGens() {
		return std::ref(animGroupSoundGens);
	}

	//
	// TES3::AnimationData
	//

	AnimationData* AnimationData::ctor() {
		// Call base class ctor.
		AnimationDataVanilla::ctor();

		// Finish construction of new members.
		::new (&customSources) decltype(customSources);
		::new (&customAnimDefinitions) decltype(customAnimDefinitions);
		::new (&namedGroups) decltype(namedGroups);
		::new (&temporarySwitchedGroups) decltype(temporarySwitchedGroups);

		// Ensure there are 3 initial sources to match vanilla.
		// customSources elements are zeroed by the SequenceGroup ctor.
		customSources.resize(VanillaSourceCount);
		customAnimDefinitions.resize(VanillaSourceCount);
		std::fill(customAnimDefinitions.begin(), customAnimDefinitions.end(), nullptr);

		// Set vanilla sequence pointers to some obvious junk data to catch accesses.
		memset(keyframeSources, 0xCD, sizeof(keyframeSources));

		return this;
	}

	const auto TES3_AnimationData_dtor = reinterpret_cast<void(__thiscall*)(AnimationData*)>(0x46B830);
	void AnimationData::dtor() {
		const auto NI_Sequence_deleting_dtor = reinterpret_cast<void(__thiscall*)(NI::Sequence*, char)>(0x4F05E0);

		// Clean up all sequences.
		if (manager) {
			manager->deactivateAll();
			manager->removeAll();
		}
		for (auto& source : customSources) {
			for (int section = 0; section < BodySectionCount; ++section) {
				auto seq = source.at(section);
				if (seq) {
					NI_Sequence_deleting_dtor(seq, 1);
				}
			}
		}

		// Release loaded animations. Leave vanilla animations.
		auto modelLoader = DataHandler::get()->nonDynamicData->meshData;
		for (int i = VanillaSourceCount; i < customAnimDefinitions.size(); ++i) {
			if (customAnimDefinitions[i]) {
				modelLoader->releaseKeyframes(customAnimDefinitions[i]);
			}
		}
		
		// Destruction of new members.
		customAnimDefinitions.~vector();
		customSources.~vector();

		// Set vanilla sequence pointers to zero before dtor.
		memset(keyframeSources, 0, sizeof(keyframeSources));

		// Call base dtor.
		TES3_AnimationData_dtor(this);
	}

	bool AnimationData::setSourceKeyframes(KeyframeDefinition* kfData, int sourceIndex, bool isBiped) {
		if (sourceIndex >= customAnimDefinitions.size()) {
			// Allocate space for the new source. The engine will write to the sequence group for the source.
			customAnimDefinitions.resize(sourceIndex + 1);
			customSources.resize(sourceIndex + 1);
		}

		// Store anim source. This is read by extension code inside setSourceKeyframes.
		customAnimDefinitions[sourceIndex] = kfData;

		// Call original function, which has some patches applied.
		bool result = TES3_AnimationData_setSourceKeyframes(this, kfData, sourceIndex, isBiped);

		if (result && isBiped) {
			// Check cloned sequences for bones that don't exist and remove them.
			for (int bodySection = 0; bodySection < BodySectionCount; ++bodySection) {
				auto seq = customSources[sourceIndex].at(bodySection);
				if (seq == nullptr) {
					continue;
				}

				// Re-implementation of remove_if on both arrays that handles TArray object lifetimes correctly.
				int removeIndex = 0;
				for (int i = 0; i < seq->objectNames.endIndex; ++i) {
					if (actorNode->getObjectByName(seq->objectNames[i])) {
						if (removeIndex < i) {
							std::swap(seq->objectNames.at(i), seq->objectNames.at(removeIndex));
							std::swap(seq->controllers.at(i), seq->controllers.at(removeIndex));
						}
						++removeIndex;
					}
				}
				for (int i = removeIndex; i < seq->objectNames.endIndex; ++i) {
					// mwse::log::getLog() << "[AnimLoader] Discarding unmatched bone '" << seq->objectNames.at(i) << "' from " << actorNode->getName() << std::endl;
					mwse::tes3::_delete(seq->objectNames.at(i));
					seq->objectNames.at(i) = nullptr;
					seq->controllers.at(i) = nullptr;
				}
				seq->objectNames.endIndex = removeIndex;
				seq->controllers.endIndex = removeIndex;
			}
		}
		return result;
	}

	void AnimationData::mergeAnimGroup(AnimationGroup* animGroup, int sourceIndex) {
		// Merge code for an individual group, based on the original implementation.
		auto groupId = animGroup->groupId;
		int i = int(groupId);

		// Start up idle animation if this is the base idle and there is no animation playing.
		if (currentAnimGroups[0] == AnimGroupID::NONE && nextAnimGroup == AnimGroupID::NONE && groupId == AnimGroupID::Idle) {
			nextAnimGroup = AnimGroupID::Idle;
		}

		// Set animation data, copying derived data from the animation group.
		// This is changed from vanilla, which did not cache these calculations.
		// AnimationGroup::calcNoteTimes and AnimationDataVanilla::calcRootMovement are now redundant.
		animationGroups[i] = animGroup;
		animGroupSourceIndices[i] = sourceIndex;
		approxRootTravelSpeeds[i] = animGroup->patchedRootTravelSpeed;

		// Clear existing soundgens.
		if (animGroupSoundGens[i]) {
			mwse::tes3::_delete(animGroupSoundGens[i]);
		}
		animGroupSoundGens[i] = nullptr;

		// Derive new soundgen array from group.
		auto soundGenCount = animGroup->soundGenCount;
		animGroupSoundGenCounts[i] = soundGenCount;
		if (soundGenCount > 0) {
			auto soundGenArray = mwse::tes3::_new<AnimationGroup::SoundGenKey*>(soundGenCount);
			for (int i = 0; i < soundGenCount; ++i) {
				soundGenArray[i] = animGroup->soundGenKeys + i;
			}
			animGroupSoundGens[i] = soundGenArray;
		}
	}

	bool AnimationData::setOverrideSourceKeyframes(KeyframeDefinition* kfData) {
		constexpr int specialSourceIndex = 0;
		bool success = setSourceKeyframes(kfData, specialSourceIndex, true);
		if (success) {
			mergeAnimGroups(kfData->animationGroups, specialSourceIndex);
		}
		return success;
	}

	void AnimationData::mergeAnimGroups(AnimationGroup* firstGroup, int sourceIndex) {
		for (auto iter = firstGroup; iter; iter = iter->nextGroup) {
			mergeAnimGroup(iter, sourceIndex);
		}
	}
	
	void AnimationData::setTemporarySwitch(TargetAnimGroup& target, int bodySection) {
		// This function switches in an animation group temporarily, which is reverted to the original when it is no longer being played.
		// The temporary switches are checked on every playGroup.
		// It is a compromise design which is easier than patching every animation group access and comparison.
		TemporarySwitchedGroup* existingSwitch = nullptr;
		auto expireSwitch = temporarySwitchedGroups.end();

		for (auto t = temporarySwitchedGroups.begin(); t != temporarySwitchedGroups.end(); ++t) {
			// Check for matching group id. There may be different animations with the same id, but only one can be active.
			if (t->temporary.group->groupId == target.group->groupId) {
				// Re-use already-active matching temporary anim group.
				t->activeSections[bodySection] = true;
				existingSwitch = &*t;
			}
			else if (t->activeSections[bodySection]) {
				// De-activate previous temporary anim group.
				t->activeSections[bodySection] = false;
				// Retire it if no section is using it.
				if (!(t->activeSections[0] || t->activeSections[1] || t->activeSections[2])) {
					expireSwitch = t;
				}
			}
		}
		if (expireSwitch != temporarySwitchedGroups.end()) {
			// The temporary anim group can be safely retired.
			revertSwitchedGroup(*expireSwitch);
			temporarySwitchedGroups.erase(expireSwitch);
		}

		if (!existingSwitch) {
			// Set up a new temporary anim group.
			int groupId = int(target.group->groupId);
			TemporarySwitchedGroup tsg{
				target,
				TargetAnimGroup{ animationGroups[groupId], animGroupSourceIndices[groupId] },
				{ false, false, false }
			};
			tsg.activeSections[bodySection] = true;

			temporarySwitchedGroups.emplace_back(tsg);
			applyTargetGroup(tsg.temporary);
		}
		else {
			// There is a matching group id. Check if this is setting a different anim group with the same group id.
			if (target.group != existingSwitch->temporary.group) {
				// Replace the anim group without regard to the other body sections. A limitation of this design.
				existingSwitch->temporary = target;
				applyTargetGroup(target);
				// Set the animation for this section to idle, so that the engine can detect the transition between two anim groups with the same id.
				currentAnimGroups[bodySection] = AnimGroupID::Idle;
			}
		}
	}

	void AnimationData::revertTemporarySwitches(int bodySection) {
		// This function will flag a body section as not using a temporary anim group, then revert any switched groups not in use by any section.
		// It is a compromise design which is easier than patching every animation group access and comparison.
		auto expireSwitch = temporarySwitchedGroups.end();

		for (auto t = temporarySwitchedGroups.begin(); t != temporarySwitchedGroups.end(); ++t) {
			if (t->activeSections[bodySection]) {
				// De-activate previous temporary anim group.
				t->activeSections[bodySection] = false;
				// Retire it if no section is using it.
				if (!(t->activeSections[0] || t->activeSections[1] || t->activeSections[2])) {
					expireSwitch = t;
				}
				break;
			}
		}
		if (expireSwitch != temporarySwitchedGroups.end()) {
			// The temporary anim group can be safely retired.
			revertSwitchedGroup(*expireSwitch);
			temporarySwitchedGroups.erase(expireSwitch);
		}
	}

	void AnimationData::revertSwitchedGroup(const TemporarySwitchedGroup& tsg) {
		// Restore the original group when a switch expires.
		if (tsg.original.group) {
			// There is an original source that can be applied.
			applyTargetGroup(tsg.original);
		}
		else {
			// There was no original group present, and the current animation may need to be reset to idle.
			auto revertGroupId = tsg.temporary.group->groupId;
			animationGroups[int(revertGroupId)] = nullptr;
			animGroupSourceIndices[int(revertGroupId)] = -1;

			for (int i = 0; i < BodySectionCount; ++i) {
				if (currentAnimGroups[i] == revertGroupId) {
					currentAnimGroups[i] = AnimGroupID::NONE;
				}
			}
		}
	}

	void AnimationData::playAnimationGroupForSection(AnimGroupID groupId, int bodySection, int startFlag, int loopCount) {
		// Prevent out-of-bounds array access.
		if (bodySection < 0 || bodySection > 2) {
			return;
		}

		// This function is a replacement for the vanilla engine function.
		// Cancel any active temporary animation for this body section.
		revertTemporarySwitches(bodySection);

		// Trigger event and original function.
		AnimationDataVanilla::playAnimationGroupForSection(groupId, bodySection, startFlag, loopCount);
	}

	void AnimationData::playNamedAnimationGroup(std::string_view name, int bodySection, int startFlag, int loopCount) {
		// Prevent out-of-bounds array access.
		if (bodySection < 0 || bodySection > 2) {
			return;
		}

		auto iter = namedGroups.find(KeyframeDefinition::toCanonicalName(name));
		if (iter == namedGroups.end()) {
			return;
		}
		auto& namedTarget = iter->second;

		// Playing named animations is automatically temporary.
		setTemporarySwitch(namedTarget, bodySection);

		// Trigger event and original function.
		AnimationDataVanilla::playAnimationGroupForSection(namedTarget.group->groupId, bodySection, startFlag, loopCount);
	}

	void AnimationData::deactivateSources() {
		// Disable sequences from current active sources.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto currentSectionSource = currentAnimGroupSources[i];
			if (currentSectionSource != -1) {
				auto seq = customSources[currentSectionSource].at(i);
				if (seq) {
					manager->deactivateSequence(seq);
				}
			}
		}
	}

	void AnimationData::reactivateSources() {
		// Enable sequences for newly updated sources.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto currentSectionSource = currentAnimGroupSources[i];
			if (currentSectionSource != -1) {
				auto seq = customSources[currentSectionSource].at(i);
				if (seq) {
					manager->activateSequence(seq);
				}
			}
		}

		// Update movement root so that the actor isn't shifted on the animation change.
		TES3::Vector3 unused;
		updateMovementDelta(timings[0], &unused, true);
	}

	bool AnimationData::applyTargetGroup(const TargetAnimGroup& targetGroup) {
		int sourceIndex = targetGroup.sourceIndex;

		// Merge in group.
		deactivateSources();
		mergeAnimGroup(targetGroup.group, sourceIndex);

		// Reset source indices and timings to the start of the action for any currently running anims, if they use the new group.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto groupId = currentAnimGroups[i];
			if (animGroupSourceIndices[int(groupId)] == sourceIndex && currentAnimGroupSources[i] != sourceIndex) {
				currentAnimGroupSources[i] = sourceIndex;
				float t = animationGroups[int(groupId)]->actionTimings[currentActionIndices[i]];
				setTiming(groupId, i, t);
			}
		}
		reactivateSources();

		return true;
	}

	bool AnimationData::hasNamedGroup(std::string_view name) {
		auto iter = namedGroups.find(KeyframeDefinition::toCanonicalName(name));
		return iter != namedGroups.end();
	}

	void AnimationData::onSectionInheritAnim(int bodySection) {
		// Called when an upper or left arm section's animation ends. It will inherit the current lower body animation.
		currentActionIndices[bodySection] = currentActionIndices[0];
		currentAnimGroups[bodySection] = currentAnimGroups[0];
		loopCounts[bodySection] = loopCounts[0];
		timings[bodySection] = timings[0];

		// Do necessary updates if the source has changed.
		int prevSource = currentAnimGroupSources[bodySection], newSource = currentAnimGroupSources[0];
		if (newSource != prevSource) {
			if (prevSource != -1) {
				manager->deactivateSequence(customSources[prevSource].at(bodySection));
			}
			manager->activateSequence(customSources[newSource].at(bodySection));
			currentAnimGroupSources[bodySection] = newSource;
		}
	}

	bool AnimationData::hasOverrideAnimations() const {
		return customSources[0].lower != nullptr;
	}

	bool AnimationData::getVanillaTarget(AnimGroupID groupId, TargetAnimGroup& out_target) {
		// Find a vanilla anim group using the original priority of sources.
		for (int v = 0; v < VanillaSourceCount; ++v) {
			auto vanillaAnim = customAnimDefinitions[v];
			if (vanillaAnim) {
				auto vanillaGroup = vanillaAnim->animationGroups->findGroup(groupId);
				if (vanillaGroup) {
					out_target.group = vanillaGroup;
					out_target.sourceIndex = v;
					return true;
				}
			}
		}
		return false;
	}

	bool AnimationData::addCustomAnim(KeyframeDefinition* kfData) {
		if (std::find(customAnimDefinitions.cbegin(), customAnimDefinitions.cend(), kfData) != customAnimDefinitions.cend()) {
			return false;
		}

		// Call source setup function that will add to customAnimDefinitions and customSources.
		int sourceIndex = customAnimDefinitions.size();
		bool isBiped = customSources[BaseAnimSourceIndex].upper != nullptr;

		kfData->refCount++;
		setSourceKeyframes(kfData, sourceIndex, isBiped);

		// Named anims are indexed separately, using namedGroups as a lookup table.
		for (auto& namedGroup : kfData->namedGroups) {
			namedGroups.emplace(namedGroup.first, TargetAnimGroup{ namedGroup.second, sourceIndex });
		}

		return true;
	}

	bool AnimationData::applyCustomAnim(std::string_view name) {
		auto iter = std::find_if(customAnimDefinitions.begin(), customAnimDefinitions.end(),
			[&](TES3::KeyframeDefinition* anim) { return anim && name == anim->filename; }
		);
		if (iter == customAnimDefinitions.end()) {
			return false;
		}
		int sourceIndex = iter - customAnimDefinitions.begin();
		auto kfData = *iter;

		// Merge in groups from this anim.
		deactivateSources();
		mergeAnimGroups(kfData->animationGroups, sourceIndex);

		// Reset source indices and timings to the start of the action for any currently running anims, if they use any of the new groups.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto groupId = int(currentAnimGroups[i]);
			if (animGroupSourceIndices[groupId] == sourceIndex && currentAnimGroupSources[i] != sourceIndex) {
				currentAnimGroupSources[i] = sourceIndex;
				float t = animationGroups[groupId]->actionTimings[currentActionIndices[i]];
				setTiming(AnimGroupID(groupId), i, t);
			}
		}
		reactivateSources();

		return true;
	}

	bool AnimationData::removeCustomAnim(std::string_view name) {
		auto iter = std::find_if(customAnimDefinitions.begin(), customAnimDefinitions.end(),
			[&](TES3::KeyframeDefinition* anim) { return anim && name == anim->filename; }
		);
		if (iter == customAnimDefinitions.end()) {
			return false;
		}
		int sourceIndex = iter - customAnimDefinitions.begin();
		auto kfData = *iter;

		deactivateSources();

		// Remove any switched anim groups that use this source.
		for (auto& tsg : temporarySwitchedGroups) {
			if (tsg.original.sourceIndex == sourceIndex) {
				// Set the original anim to a vanilla source.
				getVanillaTarget(tsg.original.group->groupId, tsg.original);
			}
			if (tsg.temporary.sourceIndex == sourceIndex) {
				// Switch back to the original.
				revertSwitchedGroup(tsg);
			}
		}
		const auto matchTemporarySourceIndex = [&](const auto& tsg) {
			return tsg.temporary.sourceIndex == sourceIndex;
		};
		auto iterErase = std::remove_if(temporarySwitchedGroups.begin(), temporarySwitchedGroups.end(), matchTemporarySourceIndex);
		temporarySwitchedGroups.erase(iterErase, temporarySwitchedGroups.end());

		// Merge in vanilla groups to replace animgroups from this anim.
		for (int groupId = 0; groupId <= int(AnimGroupID::Last); ++groupId) {
			if (animGroupSourceIndices[groupId] == sourceIndex) {
				TargetAnimGroup vanillaTarget;
				if (getVanillaTarget(AnimGroupID(groupId), vanillaTarget)) {
					mergeAnimGroup(vanillaTarget.group, vanillaTarget.sourceIndex);
				}
			}
		}

		// Reset timings to the start of the action for any currently running anims, if they were using this source.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto groupId = int(currentAnimGroups[i]);
			if (currentAnimGroupSources[i] == sourceIndex) {
				currentAnimGroupSources[i] = animGroupSourceIndices[groupId];
				float t = animationGroups[groupId]->actionTimings[currentActionIndices[i]];
				setTiming(AnimGroupID(groupId), i, t);
			}
		}
		reactivateSources();

		// Clear named groups from available names.
		for (auto& namedGroup : kfData->namedGroups) {
			namedGroups.erase(namedGroup.first);
		}

		return true;
	}

	void AnimationData::resetCustomAnims() {
		deactivateSources();

		// Merge in vanilla animations in original priority order.
		for (int layer = BaseAnimSourceIndex; layer >= 0; --layer) {
			if (customAnimDefinitions[layer]) {
				mergeAnimGroups(customAnimDefinitions[layer]->animationGroups, layer);
			}
		}

		// Reset source indices and timings to the start of the action for any currently running anims.
		for (int i = 0; i < BodySectionCount; ++i) {
			auto groupId = int(currentAnimGroups[i]);
			if (currentAnimGroupSources[i] != animGroupSourceIndices[groupId]) {
				currentAnimGroupSources[i] = animGroupSourceIndices[groupId];
				float t = animationGroups[groupId]->actionTimings[currentActionIndices[i]];
				setTiming(AnimGroupID(groupId), i, t);
			}
		}

		reactivateSources();
	}

	std::reference_wrapper<decltype(AnimationData::customSources)> AnimationData::getKeyframeSources() {
		return std::ref(customSources);
	}

	std::reference_wrapper<decltype(AnimationData::customAnimDefinitions)> AnimationData::getAnimationDefinitions() {
		return std::ref(customAnimDefinitions);
	}

	//
	// Patch: Extended animation system.
	//

#if _DEBUG
	// Debug builds add another member to vectors, increasing the offset of the data pointer.
	static constexpr auto CUSTOM_SOURCES_OFFSET_C = offsetof(AnimationData, customSources) + 0x4;
#define CUSTOM_SOURCES_OFFSET 0x7E8
#else
	static constexpr auto CUSTOM_SOURCES_OFFSET_C = offsetof(AnimationData, customSources);
#define CUSTOM_SOURCES_OFFSET 0x7E4
#endif

	__declspec(naked) NI::Sequence* addData_getLowerGroup() {
		__asm {
			mov edx, [esi + CUSTOM_SOURCES_OFFSET]	// edx = esi->customSources.begin
			lea eax, [edi + edi*2]
			mov eax, [edx + eax*4]	// eax = (edx + edi * sizeof(SequenceGroup))->lower
			ret
		}
	}
	__declspec(naked) void patchAddData() {
		__asm {
			test eax, eax
		}
	}
	const size_t patchAddData_size = 0x2;

	__declspec(naked) AnimationDataVanilla::SequenceGroup* setSourceKeyFrames_getSequenceGroup() {
		__asm {
			mov ebp, [esi + CUSTOM_SOURCES_OFFSET]	// ebp = esi->customSources.begin
			lea eax, [edi + edi*2]
			lea ebp, [ebp + eax*4]	// ebp = ebp + edi * sizeof(SequenceGroup)
			ret
		}
	}

	__declspec(naked) void patchSetSourceKeyFrames1() {
		__asm {
			mov eax, [ebp + 4]		// eax = sequenceGroup.upper
		}
	}
	const size_t patchSetSourceKeyFrames1_size = 0x3;

	__declspec(naked) void patchSetSourceKeyFrames2() {
		__asm {
			nop
			nop
			nop
			mov ecx, [ebp + 8]		// ecx = sequenceGroup.leftArm
		}
	}
	const size_t patchSetSourceKeyFrames2_size = 0x6;

	const BYTE patchLinkAnimSequences1[] = {
		0x74, 0x0B		// jmp short
	};
	const size_t patchLinkAnimSequences1_size = sizeof(patchLinkAnimSequences1);

	__declspec(naked) void patchLinkAnimSequences2() {
		__asm {
			add esp, 8
			mov edx, [esp + 0x40]		// mov edx, [esp+sourceIndex]
			lea ecx, [edx + edx*2]
			add ecx, edi				// ecx += sectionIndex
			mov edx, [esp + 0x24]		// mov edx, [esp+this]
			mov edx, [edx + CUSTOM_SOURCES_OFFSET]		// edx = edx->customSources.begin
			mov [edx + ecx*4], eax		// *sequence = eax
			jmp $ + 0x16				// jump to end of loop
		}
	}
	const size_t patchLinkAnimSequences2_size = 0x1E;

	__declspec(naked) NI::Sequence* update_getUpperSequence() {
		__asm {
			mov ecx, [ebp + CUSTOM_SOURCES_OFFSET]	// ecx = ebp->customSources.begin
			lea edx, [eax + eax*2 + 1]
			mov eax, [ecx + edx*4]	// eax = (ecx + eax * sizeof(SequenceGroup))->upper
			ret
		}
	}

	__declspec(naked) NI::Sequence* update_getLeftArmSequence() {
		__asm {
			mov ecx, [ebp + CUSTOM_SOURCES_OFFSET]	// ecx = ebp->customSources.begin
			lea edx, [eax + eax*2 + 2]
			mov eax, [ecx + edx*4]	// eax = (ecx + eax * sizeof(SequenceGroup))->leftArm
			ret
		}
	}

	__declspec(naked) NI::Sequence* calcRootMovement_getLowerSequence() {
		__asm {
			mov ecx, [esi + CUSTOM_SOURCES_OFFSET]	// ecx = esi->customSources.begin
			lea eax, [eax + eax*2]
			mov ecx, [ecx + eax*4]	// ecx = (ecx + eax * sizeof(SequenceGroup))->lower
			ret
		}
	}

	__declspec(naked) void patchCalcRootMovement() {
		__asm {
			test ecx, ecx
		}
	}
	const size_t patchCalcRootMovement_size = 0x2;

	__declspec(naked) NI::Sequence* setSequencePlayback_getSequence() {
		__asm {
			mov ecx, [esi + CUSTOM_SOURCES_OFFSET]	// ecx = esi->customSources.begin
			lea eax, [eax + eax*2]
			add eax, edi
			mov eax, [ecx + eax*4]	// eax = *(ecx + eax * sizeof(SequenceGroup) + edi * sizeof(NI::Sequence*))
			ret
		}
	}

	__declspec(naked) NI::Sequence* patchSetSequencePlayback_getSequence() {
		__asm {
			mov eax, ebp			// eax = sourceIndex
		}
	}
	const size_t patchSetSequencePlayback_getSequence_size = 2;

	__declspec(naked) NI::Sequence* setSequencePlayback_getUpperSequence() {
		__asm {
			mov ecx, [esi + CUSTOM_SOURCES_OFFSET]	// ecx = esi->customSources.begin
			lea edx, [eax + eax*2 + 1]
			mov eax, [ecx + edx*4]	// eax = (ecx + eax * sizeof(SequenceGroup))->upper
			ret
		}
	}

	__declspec(naked) NI::Sequence* setSequencePlayback_getLeftArmSequence() {
		__asm {
			mov ecx, [esi + CUSTOM_SOURCES_OFFSET]	// ecx = esi->customSources.begin
			lea edx, [eax + eax*2 + 2]
			mov eax, [ecx + edx*4]	// eax = (ecx + eax * sizeof(SequenceGroup))->leftArm
			ret
		}
	}

	__declspec(naked) NI::Sequence* setAnimationStateDirect_getSequence() {
		__asm {
			mov ecx, [esi + CUSTOM_SOURCES_OFFSET]	// ecx = esi->customSources.begin
			lea eax, [eax + eax*2]
			add eax, edi
			mov eax, [ecx + eax*4]	// eax = *(ecx + eax * sizeof(SequenceGroup) + edi * sizeof(NI::Sequence*))
			ret
		}
	}

	__declspec(naked) NI::Sequence* patchSetAnimationStateDirect() {
		__asm {
			mov eax, ebx			// eax = sourceIndex
			nop
		}
	}
	const size_t patchSetAnimationStateDirect_size = 3;

	const auto NI_Sequence_ctor = reinterpret_cast<NI::Sequence* (__thiscall*)(NI::Sequence*, const char*, unsigned int, unsigned int)>(0x70F470);
	const auto TES3_animationSequenceNames = reinterpret_cast<const char**>(0x7889CC);
	NI::Sequence* __fastcall splitBodySectionSequences_newSequence(AnimationData* _this, DWORD _EDX_, NI::Sequence* seq, int bodySection, int sourceIndex) {
		// Sequences require unique names, or their controllers will silently fail to activate.
		if (sourceIndex < VanillaSourceCount) {
			// Vanilla naming.
			auto name = TES3_animationSequenceNames[sourceIndex * BodySectionCount + bodySection];
			return NI_Sequence_ctor(seq, name, 1, 1);
		}
		else {
			// Custom naming.
			auto kfData = _this->customAnimDefinitions.at(sourceIndex);
			auto sectionName = TES3_animationSequenceNames[bodySection] + 0xD;
			char buffer[240];
			std::snprintf(buffer, sizeof(buffer), "%s (%s)", kfData->filename, sectionName);
			return NI_Sequence_ctor(seq, buffer, 1, 1);
		}
	}

	__declspec(naked) NI::Sequence* patchSplitBodySectionSequences() {
		__asm {
			push ecx				// push sourceIndex
			push edx				// push bodySection
			push eax				// push new sequence
			mov ecx, [esp + 0x30]	// set up thiscall
			jmp $ + 0xC				// jmp to call
		}
	}
	const size_t patchSplitBodySectionSequences_size = 0xC;

	__declspec(naked) void patchOnSectionInheritAnim() {
		__asm {
			push esi				// push bodySection
			mov ecx, ebp			// set up thiscall
		}
	}
	const size_t patchOnSectionInheritAnim_size = 0x3;

	//
	// Patch: Lua events from animations. Dependent on new TES3::AnimationGroup parser.
	//

	bool __fastcall animUpdateSoundEvents_checkEvent(AnimationGroup::SoundGenKey* key, Reference* reference, bool flagExecuteEvent) {
		if (!flagExecuteEvent || key->sound == nullptr) {
			return false;
		}

		const auto e = key->getLuaEvent();
		if (e) {
			// This is really a "LuaEvent:" key. Signal an event.
			if (mwse::lua::event::AnimationTriggerEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new mwse::lua::event::AnimationTriggerEvent(reference, e->id, e->param));
			}
			// Don't try to play this as a sound.
			return false;
		}

		// It's a regular sound.
		return flagExecuteEvent;
	}

	__declspec(naked) void patchAnimUpdateSoundEvents() {
		__asm {
			mov edx, [esp + 0x24]	// edx = reference
			push [esp + 0x28]		// push stack var flagPlaySound
			call $					// replace with call target
			test al, al
			jz $ + 0x35F			// skip sound if false

			xor bl, bl				// rearranged code of next block
			_emit 0x3B				// masm bug workaround. used instead of cmp esi, [0x7C86D8]
			_emit 0x35
			_emit 0xD8
			_emit 0x86
			_emit 0x7C
			_emit 0
		}
	}
	const size_t patchAnimUpdateSoundEvents_size = 0x1D;

	//
	// Patch: Parser call in vanilla KeyframeDefinition::ctor
	//
	
	__declspec(naked) void patchKeyframeDefinitionCallParser() {
		__asm {
			push ebx		// push KeyframeDefinition
		}
	}
	const size_t patchKeyframeDefinitionCallParser_size = 1;

	//
	// Patch: Allow changing cast animation speed. Custom speed is read and applied on initial cast.
	//

#pragma warning(push)
#pragma warning(disable: 4102)
	__declspec(naked) bool patchApplyAnimationSpeed() {
		__asm {
			fld [ebp + 0x64]		// ebp->AnimationData.deltaTime

		isMovementAnim:
			cmp cl, 0x2B			// AnimGroup_SwimWalkForward
			jb isWeaponAnim
			cmp cl, 0x7F			// AnimGroup_Jump2w
			ja isWeaponAnim
		movement:
			fmul [ebp + 0x4D8]		// * ebp->AnimationData.movementSpeed
			jmp done

		isWeaponAnim:
			cmp cl, 0x80			// AnimGroup_SpellCast (added test)
			je weapon
			cmp cl, 0x83			// AnimGroup_Attack1 (added creature and h2h attacks)
			jb done
			cmp cl, 0x8F			// AnimGroup_WeaponTwoWide
			ja done
		weapon:
			fmul [ebp + 0x4DC]		// * ebp->AnimationData.weaponSpeed
			nop
			nop
			nop
		done:
		}
	}
	const size_t patchApplyAnimationSpeed_size = 0x2D;
#pragma warning(pop)

	void __fastcall setAnimSpeedOnCast(TES3::AnimationData* animData) {
		// Ensure non-zero weaponSpeed to bypass the actor controller resetting the value on zero.
		animData->weaponSpeed = animData->getCastSpeed() + std::numeric_limits<float>::min();
	}

	float __fastcall updateWorldDataBeforeScale(TES3::Reference* reference) {
		reference->sceneNode->update();
		return reference->getScale();
	}

	void AnimationData::patch() {
		using mwse::genCallEnforced;
		using mwse::genCallUnprotected;
		using mwse::genNOPUnprotected;
		using mwse::writeByteUnprotected;
		using mwse::writeBytesUnprotected;
		using mwse::writeDoubleWordUnprotected;
		using mwse::writePatchCodeUnprotected;

		// Override AnimationData creation with MWSE extended struct.
		auto AnimationDataExtended_ctor = &AnimationData::ctor;
		writeDoubleWordUnprotected(0x4E6386 + 1, sizeof(AnimationData));
		genCallEnforced(0x4E63A5, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_ctor));
		writeDoubleWordUnprotected(0x4E64C7 + 1, sizeof(AnimationData));
		genCallEnforced(0x4E64E6, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_ctor));
		writeDoubleWordUnprotected(0x4E6747 + 1, sizeof(AnimationData));
		genCallEnforced(0x4E6766, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_ctor));
		writeDoubleWordUnprotected(0x4E69CD + 1, sizeof(AnimationData));
		genCallEnforced(0x4E69EC, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_ctor));
		writeDoubleWordUnprotected(0x4E6C42 + 1, sizeof(AnimationData));
		genCallEnforced(0x4E6C5F, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_ctor));

		auto AnimationDataExtended_dtor = &AnimationData::dtor;
		genCallEnforced(0x4E46E7, 0x46B830, *reinterpret_cast<DWORD*>(&AnimationDataExtended_dtor));
		genCallEnforced(0x4E4C88, 0x46B830, *reinterpret_cast<DWORD*>(&AnimationDataExtended_dtor));
		genCallEnforced(0x4E6852, 0x46B830, *reinterpret_cast<DWORD*>(&AnimationDataExtended_dtor));
		genCallEnforced(0x4E6E03, 0x46B830, *reinterpret_cast<DWORD*>(&AnimationDataExtended_dtor));
		genCallEnforced(0x4E893D, 0x46B830, *reinterpret_cast<DWORD*>(&AnimationDataExtended_dtor));

		// Override KeyframeDefinition creation with MWSE extended struct.
		auto KeyframeDefinitionExtended_ctor = &KeyframeDefinition::ctor;
		writeByteUnprotected(0x4EE24F + 1, sizeof(KeyframeDefinition));
		genCallEnforced(0x4EE26D, 0x4EDBF0, *reinterpret_cast<DWORD*>(&KeyframeDefinitionExtended_ctor));
		// Override parser call in KeyframeDefinition ctor.
		writePatchCodeUnprotected(0x4EDC87, reinterpret_cast<BYTE*>(&patchKeyframeDefinitionCallParser), patchKeyframeDefinitionCallParser_size);

		// Extend AnimationGroup dtor to clean up memory.
		auto AnimationGroup_dtor = &AnimationGroup::dtor;
		genCallEnforced(0x492863, 0x492880, *reinterpret_cast<DWORD*>(&AnimationGroup_dtor));

		// Patch: Allow changing cast animation speed. Custom speed is read and applied on initial cast.
		writePatchCodeUnprotected(0x46CAC0, reinterpret_cast<BYTE*>(&patchApplyAnimationSpeed), patchApplyAnimationSpeed_size);
		genCallUnprotected(0x541B81, reinterpret_cast<DWORD>(&setAnimSpeedOnCast), 0xA);

		// Replace text key to animation group parser.
		genCallEnforced(0x4EDC8A, 0x4C30F0, reinterpret_cast<DWORD>(&KeyframeDefinition::parseSeqTextKeysToAnimGroups));

		// Disable "Animation group note problem" warning triggered by testing custom notes.
		if constexpr (DEBUG_ANIM_PARSER) {
			writeByteUnprotected(0x4C3A71, 0xEB);
		}

		// Patch every mergeAnimGroups call.
		auto AnimationDataExtended_mergeAnimGroups = &AnimationData::mergeAnimGroups;
		genCallEnforced(0x4E646C, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6485, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6574, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6827, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E68B5, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E695A, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6AC5, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6B6A, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6BBF, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6BD8, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));
		genCallEnforced(0x4E6CED, 0x4708D0, *reinterpret_cast<DWORD*>(&AnimationDataExtended_mergeAnimGroups));

		// Patch every setSourceKeyframes call.
		auto AnimationDataExtended_setSourceKeyframes = &AnimationData::setSourceKeyframes;
		genCallEnforced(0x46B9FA, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E644C, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E6808, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E68F7, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E699D, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E6AA7, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E6B05, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E6B4B, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));
		genCallEnforced(0x4E6BA0, 0x46BA30, *reinterpret_cast<DWORD*>(&AnimationDataExtended_setSourceKeyframes));

		// Patch every keyframeSources access.
		genCallUnprotected(0x46B979, reinterpret_cast<DWORD>(addData_getLowerGroup), 0x9);
		writePatchCodeUnprotected(0x46B982, reinterpret_cast<BYTE*>(patchAddData), patchAddData_size);

		genCallUnprotected(0x46BA50, reinterpret_cast<DWORD>(setSourceKeyFrames_getSequenceGroup), 0xA);
		genNOPUnprotected(0x46BAAE, 0xA);
		writePatchCodeUnprotected(0x46BAB8, reinterpret_cast<BYTE*>(patchSetSourceKeyFrames1), patchSetSourceKeyFrames1_size);
		writePatchCodeUnprotected(0x46BAC1, reinterpret_cast<BYTE*>(patchSetSourceKeyFrames2), patchSetSourceKeyFrames2_size);

		writeBytesUnprotected(0x46BCF2, patchLinkAnimSequences1, patchLinkAnimSequences1_size);
		writePatchCodeUnprotected(0x46BCFC, reinterpret_cast<BYTE*>(patchLinkAnimSequences2), patchLinkAnimSequences2_size);

		genCallUnprotected(0x46CA17, reinterpret_cast<DWORD>(update_getUpperSequence), 0xA);
		genCallUnprotected(0x46CA33, reinterpret_cast<DWORD>(update_getLeftArmSequence), 0xA);
		genCallUnprotected(0x46E68A, reinterpret_cast<DWORD>(update_getUpperSequence), 0xA);
		genCallUnprotected(0x46E6B2, reinterpret_cast<DWORD>(update_getLeftArmSequence), 0xA);

		genCallUnprotected(0x46FDCB, reinterpret_cast<DWORD>(calcRootMovement_getLowerSequence), 0xA);
		writePatchCodeUnprotected(0x46FDD3, reinterpret_cast<BYTE*>(patchCalcRootMovement), patchCalcRootMovement_size);
		genCallUnprotected(0x46FEAE, reinterpret_cast<DWORD>(calcRootMovement_getLowerSequence), 0x9);
		genCallUnprotected(0x4700C0, reinterpret_cast<DWORD>(calcRootMovement_getLowerSequence), 0x9);
		genNOPUnprotected(0x470105, 0x3);
		genCallUnprotected(0x47010B, reinterpret_cast<DWORD>(calcRootMovement_getLowerSequence), 0x6);

		genCallUnprotected(0x470E22, reinterpret_cast<DWORD>(setSequencePlayback_getSequence), 0xB);
		genCallUnprotected(0x470E61, reinterpret_cast<DWORD>(setSequencePlayback_getSequence), 0xB);
		writePatchCodeUnprotected(0x470E7C, reinterpret_cast<BYTE*>(patchSetSequencePlayback_getSequence), patchSetSequencePlayback_getSequence_size);
		genCallUnprotected(0x470E7E, reinterpret_cast<DWORD>(setSequencePlayback_getSequence), 0x9);
		genCallUnprotected(0x4718F1, reinterpret_cast<DWORD>(setSequencePlayback_getUpperSequence), 0xA);
		writePatchCodeUnprotected(0x47190B, reinterpret_cast<BYTE*>(patchSetSequencePlayback_getSequence), patchSetSequencePlayback_getSequence_size);
		genCallUnprotected(0x47190D, reinterpret_cast<DWORD>(setSequencePlayback_getUpperSequence), 0x9);
		genCallUnprotected(0x471970, reinterpret_cast<DWORD>(setSequencePlayback_getLeftArmSequence), 0xA);
		writePatchCodeUnprotected(0x47198A, reinterpret_cast<BYTE*>(patchSetSequencePlayback_getSequence), patchSetSequencePlayback_getSequence_size);
		genCallUnprotected(0x47198C, reinterpret_cast<DWORD>(setSequencePlayback_getLeftArmSequence), 0x9);

		genCallUnprotected(0x471A84, reinterpret_cast<DWORD>(setAnimationStateDirect_getSequence), 0xB);
		writePatchCodeUnprotected(0x471A9F, reinterpret_cast<BYTE*>(patchSetAnimationStateDirect), patchSetAnimationStateDirect_size);
		genCallUnprotected(0x471AA2, reinterpret_cast<DWORD>(setAnimationStateDirect_getSequence), 0x8);

		// This ensures that the first update of world data of NiBSParticleNode happens pre-scale, same as vanilla.
		genCallUnprotected(0x4E8331, reinterpret_cast<DWORD>(&updateWorldDataBeforeScale), 0x6);

		// Patch sequence cloning to avoid out of bounds read + SequenceManager requires unique sequence names.
		writePatchCodeUnprotected(0x46BBC6, reinterpret_cast<BYTE*>(patchSplitBodySectionSequences), patchSplitBodySectionSequences_size);
		genCallUnprotected(0x46BBD9, reinterpret_cast<DWORD>(splitBodySectionSequences_newSequence));

		// Patch source management after an animation ends, where upper and left arm sections inherit the lower section animation.
		auto AnimationDataExtended_onSectionInheritAnim = &AnimationData::onSectionInheritAnim;
		writePatchCodeUnprotected(0x46DA12, reinterpret_cast<BYTE*>(&patchOnSectionInheritAnim), patchOnSectionInheritAnim_size);
		genCallUnprotected(0x46DA15, *reinterpret_cast<DWORD*>(&AnimationDataExtended_onSectionInheritAnim), 0x12);
		writePatchCodeUnprotected(0x46E639, reinterpret_cast<BYTE*>(&patchOnSectionInheritAnim), patchOnSectionInheritAnim_size);
		genCallUnprotected(0x46E63C, *reinterpret_cast<DWORD*>(&AnimationDataExtended_onSectionInheritAnim), 0x12);

		// Patch animation sound events to also trigger lua events.
		writePatchCodeUnprotected(0x46F553, reinterpret_cast<BYTE*>(&patchAnimUpdateSoundEvents), patchAnimUpdateSoundEvents_size);
		genCallUnprotected(0x46F553 + 0x8, reinterpret_cast<DWORD>(animUpdateSoundEvents_checkEvent));
	}

}
