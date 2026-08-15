#pragma once

#include "CSPhysicalObject.h"

#include "TES3WeaponFlags.h"

namespace se::cs {
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

		bool getMaterialFlag(TES3::WeaponMaterialFlag::Flag flag) const;
		bool getIsSilver() const;
		bool getIgnoresResistance() const;

		bool isProjectile() const;
	};
	static_assert(sizeof(Weapon) == 0x7C, "Weapon failed size validation");
}
