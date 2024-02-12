#pragma once

#include "NITimeController.h"
#include "NIAnimationData.h"
#include "NIAnimationKey.h"

namespace NI {
	struct KeyframeData : Object {
		unsigned int rotationKeyCount; // 0x8
		AmbiguousRotKeyPtr rotationKeys; // 0xC
		AnimationKey::KeyType rotationType; // 0x10
		unsigned int positionKeyCount; // 0x14
		AmbiguousPosKeyPtr positionKeys; // 0x18
		AnimationKey::KeyType positionType; // 0x1C
		unsigned int scaleKeyCount; // 0x20
		AmbiguousFloatKeyPtr scaleKeys; // 0x24
		AnimationKey::KeyType scaleType; // 0x28

		sol::object getRotationKeys_lua(sol::this_state L);
		sol::object getPositionKeys_lua(sol::this_state L);
		sol::object getScaleKeys_lua(sol::this_state L);

		sol::optional<int> getRotationKeyIndex_lua(sol::this_state L, float time);
		sol::optional<int> getPositionKeyIndex_lua(sol::this_state L, float time);
		sol::optional<int> getScaleKeyIndex_lua(sol::this_state L, float time);

		void updateDerivedValues();
		void replaceScaleData(FloatKey* keys, unsigned int numKeys, AnimationKey::KeyType keyType);
	};
	static_assert(sizeof(KeyframeData) == 0x2C, "NI::KeyframeData failed size validation");

	struct KeyframeController : TimeController {
		unsigned int lastRotIndex; // 0x34
		unsigned int lastPosIndex; // 0x38
		unsigned int lastScaleIndex; // 0x3C
		Pointer<KeyframeData> keyframeData; // 0x40
	};
	static_assert(sizeof(KeyframeController) == 0x44, "NI::KeyframeController failed size validation");
}
