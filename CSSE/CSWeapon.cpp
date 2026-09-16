#include "CSWeapon.h"

namespace se::cs {
	bool Weapon::getMaterialFlag(TES3::WeaponMaterialFlag::Flag flag) const {
		return (materialFlags & flag) == flag;
	}

	bool Weapon::getIsSilver() const {
		return getMaterialFlag(TES3::WeaponMaterialFlag::Silver);
	}

	bool Weapon::getIgnoresResistance() const {
		return getMaterialFlag(TES3::WeaponMaterialFlag::IgnoresNormalWeaponResistance);
	}

	bool Weapon::isProjectile() const {
		return weaponType >= TES3::WeaponType::Thrown;
	}
}
