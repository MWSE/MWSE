#include "LuaProjectileExpireEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

namespace mwse::lua::event {
	ProjectileExpireEvent::ProjectileExpireEvent(TES3::MobileProjectile* projectile) :
		ObjectFilteredEvent("projectileExpire", projectile->firingWeapon),
		m_Projectile(projectile)
	{
		// Make sure that we have a firing actor before putting it.
		if (m_Projectile->firingActor) {
			m_EventFilter = m_Projectile->firingActor->reference;
		}
	}

	sol::table ProjectileExpireEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_Projectile;

		// Give a shorthand to the firing reference.
		if (m_Projectile->firingActor && m_Projectile->firingActor->reference) {
			eventData["firingReference"] = m_Projectile->firingActor->reference;
		}

		// Also give a shorthand to the firing weapon.
		if (m_Projectile->firingWeapon) {
			eventData["firingWeapon"] = m_Projectile->firingWeapon;
		}

		return eventData;
	}
}
