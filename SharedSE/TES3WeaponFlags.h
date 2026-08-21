#pragma once

namespace TES3 {
	namespace WeaponType {
		typedef unsigned char value_type;

		enum WeaponType : value_type {
			ShortBlade1H = 0x0,
			LongBlade1H = 0x1,
			LongBlade2H = 0x2,
			Blunt1H = 0x3,
			Blunt2close = 0x4,
			Blunt2wide = 0x5,
			Spear2H = 0x6,
			Axe1H = 0x7,
			Axe2H = 0x8,
			Bow = 0x9,
			Crossbow = 0xA,
			Thrown = 0xB,
			Arrow = 0xC,
			Bolt = 0xD
		};
	}

	namespace WeaponMaterialFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			IgnoresNormalWeaponResistance = 0x1,
			Silver = 0x2
		};

		enum FlagBit {
			IgnoresNormalWeaponResistanceBit = 0,
			SilverBit = 1
		};
	}
}
