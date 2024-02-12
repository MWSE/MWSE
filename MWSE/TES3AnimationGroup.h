#pragma once

#include "TES3Object.h"

namespace TES3 {
	struct AnimationGroup : BaseObject {
		struct SoundGenKey {
			int startFrame; // 0x0
			float startTime; // 0x4
			unsigned char volume; // 0x8
			float pitch; // 0xC
			Sound* sound; // 0x10

			SoundGenKey() = delete;
			~SoundGenKey() = delete;
		};

		unsigned char groupId; // 0x10
		unsigned int actionCount; // 0x14
		int* actionFrames; // 0x18
		float* actionTimings; // 0x1C
		AnimationGroup* nextGroup; // 0x20
		unsigned int soundGenCount; // 0x24
		SoundGenKey* soundGenKeys; // 0x28

		AnimationGroup() = delete;
		~AnimationGroup() = delete;

		nonstd::span<int> getActionFrames();
		nonstd::span<float> getActionTimings();
		nonstd::span<SoundGenKey> getSoundGenKeys();
	};
	static_assert(sizeof(AnimationGroup) == 0x2C, "TES3::AnimationGroup failed size validation");
	static_assert(sizeof(AnimationGroup::SoundGenKey) == 0x14, "TES3::AnimationGroup::SoundGenKey failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::AnimationGroup)
