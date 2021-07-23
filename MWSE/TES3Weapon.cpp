#include "TES3Weapon.h"

#include "TES3DataHandler.h"

#include "TES3Util.h"

#include "BitUtil.h"

namespace TES3 {
	const auto TES3_Weapon_ctor = reinterpret_cast<void(__thiscall*)(Weapon*)>(0x4F1EE0);
	Weapon::Weapon() {
		TES3_Weapon_ctor(this);
	}

	const auto TES3_Weapon_dtor = reinterpret_cast<void(__thiscall*)(Weapon*)>(0x4F1FA0);
	Weapon::~Weapon() {
		TES3_Weapon_dtor(this);
	}

	bool Weapon::isOneHanded() const {
		switch (weaponType)
		{
		case WeaponType::ShortBlade1H:
		case WeaponType::LongBlade1H:
		case WeaponType::Blunt1H:
		case WeaponType::Axe1H:
		case WeaponType::Thrown:
			return true;
		}
		return false;
	}

	bool Weapon::isTwoHanded() const {
		switch (weaponType)
		{
		case WeaponType::LongBlade2H:
		case WeaponType::Axe2H:
		case WeaponType::Spear2H:
		case WeaponType::Blunt2close:
		case WeaponType::Blunt2wide:
			return true;
		}
		return false;
	}

	bool Weapon::isMelee() const {
		return weaponType <= WeaponType::Axe2H;
	}

	bool Weapon::isRanged() const {
		return weaponType >= WeaponType::Bow && weaponType <= WeaponType::Thrown;
	}

	bool Weapon::isAmmo() const {
		return weaponType == WeaponType::Arrow || weaponType == WeaponType::Bolt;
	}

	bool Weapon::isProjectile() const {
		return weaponType > TES3::WeaponType::Crossbow;
	}

	bool Weapon::hasDurability() const {
		return weaponType <= TES3::WeaponType::Crossbow;
	}

	void Weapon::setDurability(int value) {
		maxCondition = value;
	}

	bool Weapon::getMaterialFlag(WeaponMaterialFlag::WeaponMaterialFlag flag) const {
		return BIT_TEST(materialFlags, flag);
	}

	void Weapon::setMaterialFlag(WeaponMaterialFlag::WeaponMaterialFlag flag, bool value) {
		BIT_SET(materialFlags, flag, value);
	}

	bool Weapon::getIgnoresNormalWeaponResistance() const {
		return getMaterialFlag(WeaponMaterialFlag::IgnoresNormalWeaponResistance);
	}

	void Weapon::setIgnoresNormalWeaponResistance(bool value) {
		setMaterialFlag(WeaponMaterialFlag::IgnoresNormalWeaponResistance, value);
	}

	bool Weapon::getIsSilver() const {
		return getMaterialFlag(WeaponMaterialFlag::Silver);
	}

	void Weapon::setIsSilver(bool) {
		setMaterialFlag(WeaponMaterialFlag::Silver, value);
	}

	int Weapon::getSkillId() const {
		return _typeToSkillMap[weaponType];
	}

	Skill* Weapon::getSkill() const {
		return &DataHandler::get()->nonDynamicData->skills[getSkillId()];
	}

	void Weapon::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Weapon)
