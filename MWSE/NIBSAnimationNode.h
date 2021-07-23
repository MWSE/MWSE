#pragma once

#include "NINode.h"

namespace NI {
	struct BSAnimationNode : Node {
		enum AVObjectFlag : unsigned char {
			Animated = 0x20,
			NotRandom = 0x40,
		};

		enum AVObjectFlagBit : unsigned char {
			AnimatedBit = 5,
			NotRandomBit = 6,
		};

		enum AnimationFlag : unsigned int {
			FirstTime = 0x2,
			Managed = 0x4,
			Displayed = 0x8,
			AlwaysUpdate = 0x10,
		};

		enum AnimationFlagBit : unsigned int {
			FirstTimeBit = 1,
			ManagedBit = 2,
			DisplayedBit = 3,
			AlwaysUpdateBit = 4,
		};

		unsigned char animationFlags; // 0xB0

		static constexpr auto getPhaseInit() { return *reinterpret_cast<float*>(0x7DE9B4); };
	};
	static_assert(sizeof(BSAnimationNode) == 0xB4, "NI::BSAnimationNode failed size validation");
}
