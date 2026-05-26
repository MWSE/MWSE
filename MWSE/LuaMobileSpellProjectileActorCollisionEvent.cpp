#include "LuaMobileSpellProjectileActorCollisionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobileSpellProjectile.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MobileSpellProjectileActorCollisionEvent::MobileSpellProjectileActorCollisionEvent(TES3::MobileSpellProjectile* projectile, TES3::Reference* targetReference, NI::Point3& point, NI::Point3& pos, NI::Point3& vel) :
		ObjectFilteredEvent("spellProjectileHitActor", projectile->firingActor ? projectile->firingActor->reference : nullptr),
		m_Projectile(projectile),
		m_TargetReference(targetReference),
		m_CollisionPoint(point),
		m_Position(pos),
		m_Velocity(vel)
	{

	}

	sol::table MobileSpellProjectileActorCollisionEvent::createEventTable() {
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

		return eventData;
	}
}
