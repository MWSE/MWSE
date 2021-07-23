#pragma once

#include "NITimeController.h"

namespace NI {
	struct AnimationKey {
		float timing; // 0x0
		unsigned char size; // 0x4
	};
	static_assert(sizeof(AnimationKey) == 0x8, "TES3::AnimationKey failed size validation");

	struct FloatKey : AnimationKey {
		float value; // 0x8
	};
	static_assert(sizeof(FloatKey) == 0xC, "TES3::FloatKey failed size validation");

	struct KeyframeData : Object {
		unsigned int rotationKeyCount; // 0x8
		void* rotationKeys; // 0xC
		int rotationType; // 0x10
		unsigned int positionKeyCount; // 0x14
		void* positionKeys; // 0x18
		int positionType; // 0x1C
		unsigned int scaleKeyCount; // 0x20
		void* scaleKeys; // 0x24
		int scaleType; // 0x28
	};
	static_assert(sizeof(KeyframeData) == 0x2C, "TES3::KeyframeData failed size validation");

	struct KeyframeController : TimeController {
		int unknown_0x34;
		int unknown_0x38;
		int unknown_0x3C;
		Pointer<KeyframeData> keyframeData; // 0x40
		int unknown_0x44;
	};
	static_assert(sizeof(KeyframeController) == 0x48, "TES3::KeyframeController failed size validation");
}
