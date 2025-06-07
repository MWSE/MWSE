#include "CSWeapon.h"

namespace se::cs {
	bool Weapon::getMaterialFlag(WeaponMaterialFlag::Flag flag) const {
		return (materialFlags & flag) == flag;
	}

	bool Weapon::getIsSilver() const {
		return getMaterialFlag(WeaponMaterialFlag::Silver);
	}

	bool Weapon::getIgnoresResistance() const {
		return getMaterialFlag(WeaponMaterialFlag::IgnoresNormalWeaponResistance);
	}

	bool Weapon::isProjectile() const {
		return weaponType >= WeaponType::Thrown;
	}
}
