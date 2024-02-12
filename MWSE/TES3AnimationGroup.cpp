#include "TES3AnimationGroup.h"

namespace TES3 {
	nonstd::span<int> AnimationGroup::getActionFrames() {
		return { actionFrames, actionCount };
	}

	nonstd::span<float> AnimationGroup::getActionTimings() {
		return { actionTimings, actionCount };
	}

	nonstd::span<AnimationGroup::SoundGenKey> AnimationGroup::getSoundGenKeys() {
		return { soundGenKeys, soundGenCount };
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::AnimationGroup)
