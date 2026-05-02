#pragma once

#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Weapon.h"

#include "TES3MobileObjectLua.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForTES3MobileProjectile(sol::usertype<T>& usertypeDefinition) {
		setUserdataForTES3MobileObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["animTime"] = &TES3::MobileProjectile::animTime;
		usertypeDefinition["attackSwing"] = &TES3::MobileProjectile::attackSwing;
		usertypeDefinition["damage"] = &TES3::MobileProjectile::damage;
		usertypeDefinition["expire"] = &TES3::MobileProjectile::flagExpire;
		usertypeDefinition["firingMobile"] = &TES3::MobileProjectile::firingActor;
		usertypeDefinition["firingWeapon"] = sol::readonly_property(&TES3::MobileProjectile::firingWeapon);
		usertypeDefinition["initialSpeed"] = sol::readonly_property(&TES3::MobileProjectile::initialSpeed);
		usertypeDefinition["velocity"] = sol::property(&TES3::MobileProjectile::getProjectileVelocity, &TES3::MobileProjectile::setProjectileVelocity_lua);
	}

	void bindTES3MobileProjectile();
}
