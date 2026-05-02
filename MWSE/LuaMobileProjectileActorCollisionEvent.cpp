#include "LuaMobileProjectileActorCollisionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

namespace mwse::lua::event {
	MobileProjectileActorCollisionEvent::MobileProjectileActorCollisionEvent(TES3::MobileProjectile* projectile, TES3::Reference* targetReference, TES3::Vector3& point, TES3::Vector3& pos, TES3::Vector3& vel) :
		ObjectFilteredEvent("projectileHitActor", projectile->firingActor->reference),
		m_Projectile(projectile),
		m_TargetReference(targetReference),
		m_CollisionPoint(point),
		m_Position(pos),
		m_Velocity(vel)
	{

	}

	sol::table MobileProjectileActorCollisionEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_Projectile;
		eventData["target"] = m_TargetReference;
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
