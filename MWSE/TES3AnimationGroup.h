#pragma once

#include "TES3Object.h"

namespace TES3 {
	struct AnimationGroup : BaseObject {
		struct Node {
			int startIndex; // 0x0
			float timeIndex; // 0x4
			unsigned char volume; // 0x8
			float pitch; // 0xC
			Sound* sound; // 0x10
		};
		unsigned char groupId; // 0x10
		unsigned int frameCount; // 0x14
		int* frames; // 0x18
		float* frameData; // 0x1C
		AnimationGroup* nextGroup; // 0x20
		unsigned int nodeCount; // 0x24
		Node* nodes; // 0x28
	};
	static_assert(sizeof(AnimationGroup) == 0x2C, "TES3::AnimationAttachment failed size validation");
	static_assert(sizeof(AnimationGroup::Node) == 0x14, "TES3::AnimationAttachment::Node failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::AnimationGroup)
