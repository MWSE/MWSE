#include "TES3AnimationGroup.h"

namespace TES3 {
	std::span<int> AnimationGroup::getActionFrames() {
		return { actionFrames, actionCount };
	}

	std::span<float> AnimationGroup::getActionTimings() {
		return { actionTimings, actionCount };
	}

	std::span<AnimationGroup::SoundGenKey> AnimationGroup::getSoundGenKeys() {
		return { soundGenKeys, soundGenCount };
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::AnimationGroup)
