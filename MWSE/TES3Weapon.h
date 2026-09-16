#pragma once

#include "TES3Defines.h"
#include "TES3WeaponFlags.h"

#include "TES3Item.h"
#include "NIIteratedList.h"

namespace TES3 {
	struct Weapon : Item {
		NI::IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char * name; // 0x44
		Script * script; // 0x48
		char * model; // 0x4C
		char * icon; // 0x50
		float weight; // 0x54
		int value; // 0x58
		WeaponType::value_type weaponType; // 0x5C
		unsigned short maxCondition; // 0x5E
		float speed; // 0x60
		float reach; // 0x64
		unsigned short enchantCapacity; // 0x68
		unsigned char chopMin; // 0x6A
		unsigned char chopMax; // 0x6B
		unsigned char slashMin; // 0x6C
		unsigned char slashMax; // 0x6D
		unsigned char thrustMin; // 0x6E
		unsigned char thrustMax; // 0x6F
		unsigned int materialFlags; // 0x70
		Enchantment * enchantment; // 0x74

		static constexpr auto OBJECT_TYPE = ObjectType::Weapon;

		Weapon();
		~Weapon();

		//
		// Custom functions.
		//

		bool isOneHanded() const;
		bool isTwoHanded() const;
		bool isMelee() const;
		bool isRanged() const;
		bool isAmmo() const;
		bool isProjectile() const;
		bool hasDurability() const;
		void setDurability(int value);

		bool getMaterialFlag(WeaponMaterialFlag::FlagBit flag) const;
		void setMaterialFlag(WeaponMaterialFlag::FlagBit flag, bool value);

		bool getIgnoresNormalWeaponResistance() const;
		void setIgnoresNormalWeaponResistance(bool value);
		bool getIsSilver() const;
		void setIsSilver(bool value);

		int getSkillId() const;
		Skill* getSkill() const;

		void setIconPath(const char* path);

		//
		// Access to this type's raw functions.
		//

		static constexpr auto _typeToSkillMap = reinterpret_cast<int*>(0x7A8C0C);
	};
	static_assert(sizeof(Weapon) == 0x78, "TES3::Weapon failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Weapon)
