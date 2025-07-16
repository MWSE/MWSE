#include "TES3AnimationData.h"
#include "TES3ActorAnimationController.h"

#include "NINode.h"
#include "NIKeyframeManager.h"

#include "LuaManager.h"

#include "LuaPlayAnimationGroupEvent.h"

namespace TES3 {
	constexpr float fixedPointSpeedScale = 256.0f;

	const auto TES3_AnimationData_ctor = reinterpret_cast<void(__thiscall*)(AnimationData*)>(0x46B7A0);
	AnimationData* AnimationData::ctor() {
		// Call original function.
		TES3_AnimationData_ctor(this);

		// Initialize custom data.
		patchedCastSpeed = (unsigned short)(1 * fixedPointSpeedScale);

		return this;
	}

	const auto TES3_AnimationData_playAnimationGroupForIndex = reinterpret_cast<void(__thiscall*)(AnimationData*, int, int, int, int)>(0x470AE0);
	void AnimationData::playAnimationGroupForIndex(int animationGroup, int triIndex, int startFlag, int loopCount) {
		if (mwse::lua::event::PlayAnimationGroupEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new mwse::lua::event::PlayAnimationGroupEvent(this, animationGroup, triIndex, startFlag, loopCount));
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

		TES3_AnimationData_playAnimationGroupForIndex(this, animationGroup, triIndex, startFlag, loopCount);
	}

	const auto TES3_AnimationData_setHeadNode = reinterpret_cast<void(__thiscall*)(AnimationData*, NI::Node*)>(0x4704B0);
	void AnimationData::setHeadNode(NI::Node* head) {
		TES3_AnimationData_setHeadNode(this, head);
	}

	const auto TES3_AnimationData_headTracking = reinterpret_cast<void(__thiscall*)(AnimationData*, Reference*, Reference*)>(0x46F910);
	void AnimationData::headTracking(Reference* actorRefr, Reference* targetRefr) {
		TES3_AnimationData_headTracking(this, actorRefr, targetRefr);
	}

	const auto TES3_AnimationData_updateMovementDelta = reinterpret_cast<bool(__thiscall*)(AnimationData*, float, Vector3*, bool)>(0x470320);
	void AnimationData::updateMovementDelta(float timing, Vector3 *inout_startingPosition, bool dontUpdatePositionDelta) {
		TES3_AnimationData_updateMovementDelta(this, timing, inout_startingPosition, dontUpdatePositionDelta);
	}

	Reference* AnimationData::getReference() const {
		if (actorNode) {
			return actorNode->getTes3Reference(false);
		}
		return nullptr;
	}

	void AnimationData::playAnimationGroup(int animationGroup, int startFlag, int loopCount) {
		playAnimationGroupForIndex(animationGroup, 0, startFlag, loopCount);
		playAnimationGroupForIndex(animationGroup, 1, startFlag, loopCount);
		playAnimationGroupForIndex(animationGroup, 2, startFlag, loopCount);
	}

	const auto TES3_AnimationData_setLayerKeyframes = reinterpret_cast<bool(__thiscall*)(AnimationData*, KeyframeDefinition*, int, bool)>(0x46BA30);
	const auto TES3_AnimationData_mergeAnimGroups = reinterpret_cast<void(__thiscall*)(AnimationData*, AnimationGroup*, int)>(0x4708D0);

	bool AnimationData::setOverrideLayerKeyframes(KeyframeDefinition* kfData) {
		constexpr int specialLayerIndex = 0;
		bool success = TES3_AnimationData_setLayerKeyframes(this, kfData, specialLayerIndex, true);
		if (success) {
			TES3_AnimationData_mergeAnimGroups(this, kfData->animationGroup, specialLayerIndex);
		}
		return success;
	}

	bool AnimationData::hasOverrideAnimations() const {
		return keyframeLayers[0].lower != nullptr;
	}

	void AnimationData::swapAnimationGroups(int animationGroup1, int animationGroup2) {
		// Swap all animation group specific data.
		std::swap(animationGroups[animationGroup1], animationGroups[animationGroup2]);
		std::swap(animGroupLayerIndices[animationGroup1], animGroupLayerIndices[animationGroup2]);
		std::swap(animGroupSoundGens[animationGroup1], animGroupSoundGens[animationGroup2]);
		std::swap(animGroupSoundGenCounts[animationGroup1], animGroupSoundGenCounts[animationGroup2]);
		std::swap(approxRootTravelDistances[animationGroup1], approxRootTravelDistances[animationGroup2]);

		// Fix up timing and sequence activation if the swap affects the currently playing animation.
		for (int i = 0; i < 3; ++i) {
			auto group = currentAnimGroup[i];
			auto sequenceGroup = &this->keyframeLayers[i].lower;

			if (group == animationGroup1 || group == animationGroup2) {
				// Reset timing to the start of the current action.
				timing[i] = animationGroups[group]->actionTimings[currentActionIndices[i]];

				int currentLayer = currentAnimGroupLayer[i], newLayer = animGroupLayerIndices[group];
				if (currentLayer != newLayer) {
					if (currentLayer != -1) {
						manager->deactivateSequence(sequenceGroup[currentLayer]);
					}
					manager->activateSequence(sequenceGroup[newLayer]);
					currentAnimGroupLayer[i] = newLayer;
				}
			}
		}

		// Fix up movement root if the swap affects the currently playing animation.
		auto lowerGroup = currentAnimGroup[0];
		if (lowerGroup == animationGroup1 || lowerGroup == animationGroup2) {
			Vector3 unused;
			updateMovementDelta(timing[0], &unused, true);
		}
	}

	float AnimationData::getCastSpeed() const {
		return patchedCastSpeed / fixedPointSpeedScale;
	}

	void AnimationData::setCastSpeed(float speed) {
		speed = std::max(0.0f, std::min(255.0f, speed));
		patchedCastSpeed = (unsigned short)(speed * fixedPointSpeedScale);

		// Update current animation speed if currently casting.
		constexpr unsigned char spellCastAnimID = 0x80;
		if (currentAnimGroup[1] == spellCastAnimID) {
			// Ensure non-zero weaponSpeed to bypass the actor controller resetting the value on zero.
			weaponSpeed = speed + FLT_MIN;
		}
	}

	std::reference_wrapper<decltype(AnimationData::currentAnimGroup)> AnimationData::getCurrentAnimGroups() {
		return std::ref(currentAnimGroup);
	}

	std::reference_wrapper<decltype(AnimationData::currentActionIndices)> AnimationData::getCurrentActionIndices() {
		return std::ref(currentActionIndices);
	}

	std::reference_wrapper<decltype(AnimationData::loopCounts)> AnimationData::getLoopCounts() {
		return std::ref(loopCounts);
	}

	std::reference_wrapper<decltype(AnimationData::timing)> AnimationData::getTimings() {
		return std::ref(timing);
	}

	std::reference_wrapper<decltype(AnimationData::animationGroups)> AnimationData::getAnimationGroups() {
		return std::ref(animationGroups);
	}

	std::reference_wrapper<decltype(AnimationData::keyframeLayers)> AnimationData::getKeyframeLayers() {
		return std::ref(keyframeLayers);
	}

	std::reference_wrapper<decltype(AnimationData::currentAnimGroupLayer)> AnimationData::getCurrentAnimGroupLayers() {
		return std::ref(currentAnimGroupLayer);
	}

	std::reference_wrapper<decltype(AnimationData::animGroupLayerIndices)> AnimationData::getAnimGroupLayerIndices() {
		return std::ref(animGroupLayerIndices);
	}

	std::reference_wrapper<decltype(AnimationData::approxRootTravelDistances)> AnimationData::getApproxRootTravelDistances() {
		return std::ref(approxRootTravelDistances);
	}

	std::reference_wrapper<decltype(AnimationData::currentSoundGenIndices)> AnimationData::getCurrentSoundGenIndices() {
		return std::ref(currentSoundGenIndices);
	}

	std::reference_wrapper<decltype(AnimationData::animGroupSoundGenCounts)> AnimationData::getAnimGroupSoundGenCounts() {
		return std::ref(animGroupSoundGenCounts);
	}

	std::reference_wrapper<decltype(AnimationData::animGroupSoundGens)> AnimationData::getAnimGroupSoundGens() {
		return std::ref(animGroupSoundGens);
	}
}
