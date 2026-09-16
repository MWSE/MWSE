#pragma once

namespace TES3 {
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
}
