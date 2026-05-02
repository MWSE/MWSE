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

		BSAnimationNode();

		static Pointer<BSAnimationNode> create();

		static constexpr auto getPhaseInit() { return *reinterpret_cast<float*>(0x7DE9B4); };
	};
	static_assert(sizeof(BSAnimationNode) == 0xB4, "NI::BSAnimationNode failed size validation");

	struct BSParticleNode : BSAnimationNode {
		enum AVObjectFlag : unsigned char {
			Follow = 0x80,
		};

		enum AVObjectFlagBit : unsigned char {
			FollowBit = 7,
		};

		BSParticleNode();

		static Pointer<BSParticleNode> create();

		bool getFollowMode() const;
		void setFollowMode(bool enable);
	};
	static_assert(sizeof(BSParticleNode) == 0xB4, "NI::BSParticleNode failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::BSAnimationNode)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::BSParticleNode)
