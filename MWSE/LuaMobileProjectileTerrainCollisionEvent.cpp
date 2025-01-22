#include "LuaMobileProjectileTerrainCollisionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

namespace mwse::lua::event {
	MobileProjectileTerrainCollisionEvent::MobileProjectileTerrainCollisionEvent(TES3::MobileProjectile* projectile, TES3::Vector3& point, TES3::Vector3& pos, TES3::Vector3& vel) :
		ObjectFilteredEvent("projectileHitTerrain", projectile->firingActor->reference),
		m_Projectile(projectile),
		m_CollisionPoint(point),
		m_Position(pos),
		m_Velocity(vel)
	{

	}

	sol::table MobileProjectileTerrainCollisionEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_Projectile;
		eventData["collisionPoint"] = m_CollisionPoint;
		eventData["position"] = m_Position;
		eventData["velocity"] = m_Velocity;

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
