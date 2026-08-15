#pragma once

namespace TES3 {
	namespace ArmorSlot {
		typedef unsigned int value_type;

		enum ArmorSlot : value_type {
			Helmet = 0x0,
			Cuirass = 0x1,
			LeftPauldron = 0x2,
			RightPauldron = 0x3,
			Greaves = 0x4,
			Boots = 0x5,
			LeftGauntlet = 0x6,
			RightGauntlet = 0x7,
			Shield = 0x8,
			LeftBracer = 0x9,
			RightBracer = 0xA,

			// Unusued in Morrowind itself, only used for MWSE operations.
			Invalid = 0xFFFFFFFF,

			First = Helmet,
			Last = RightBracer,
		};
	}

	namespace ArmorWeightClass {
		typedef unsigned int value_type;

		enum ArmorWeightClass : value_type {
			Light = 0x0,
			Medium = 0x1,
			Heavy = 0x2,

			First = Light,
			Last = Heavy,
		};
	}
}
