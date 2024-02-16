#pragma once

#include "CSActor.h"

namespace se::cs {
	namespace ActorFlagContainer {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Organic = 0x1,
			Respawns = 0x2,
			IsBase = 0x8,
		};

		enum FlagBig {
			OrganicBit = 0,
			RespawnsBit = 1,
			IsBaseBit = 2,
		};
	}

	struct Container : Actor {
		int unknown_0x7C;
		int unknown_0x80;
		const char* name; // 0x84
		const char* model; // 0x88
		Script* script; // 0x8C
		float capacity; // 0x90

		bool getIsOrganic() const;
	};
	static_assert(sizeof(Container) == 0x94, "Container failed size validation");
}
