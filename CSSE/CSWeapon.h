#pragma once

#include "CSPhysicalObject.h"

namespace se::cs {
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

	struct Weapon : PhysicalObject {
		const char* name; // 0x48
		Script* script; // 0x4C
		const char* model; // 0x50
		char* icon; // 0x54
		float weight; // 0x58
		int value; // 0x5C
		unsigned short weaponType; // 0x60
		unsigned short maxCondition; // 0x62
		float speed; // 0x64
		float reach; // 0x68
		unsigned short enchantPoints; // 0x6C
		unsigned char chopMin; // 0x6E
		unsigned char chopMax; // 0x6F
		unsigned char slashMin; // 0x70
		unsigned char slashMax; // 0x71
		unsigned char thrustMin; // 0x72
		unsigned char thrustMax; // 0x73
		unsigned int materialFlags; // 0x74
		Enchantment* enchantment; // 0x78

		bool getMaterialFlag(WeaponMaterialFlag::Flag flag) const;
		bool getIsSilver() const;
		bool getIgnoresResistance() const;

		bool isProjectile() const;
	};
	static_assert(sizeof(Weapon) == 0x7C, "Weapon failed size validation");
}
