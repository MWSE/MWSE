#pragma once

namespace TES3 {
	namespace RaceFlag {
		enum Flag : unsigned int {
			Playable = 0x1,
			Beast = 0x2,
		};
		enum FlagBit : unsigned int {
			PlayableBit = 0,
			BeastBit = 1,
		};
	}
}
