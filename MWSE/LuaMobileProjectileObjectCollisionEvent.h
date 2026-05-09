#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "NIVector3.h"

namespace mwse::lua::event {
	class MobileProjectileObjectCollisionEvent : public ObjectFilteredEvent, public DisableableEvent<MobileProjectileObjectCollisionEvent> {
	public:
		MobileProjectileObjectCollisionEvent(TES3::MobileProjectile* projectile, TES3::Reference* targetReference, NI::Vector3& point, NI::Vector3& pos, NI::Vector3& vel);
		sol::table createEventTable();

	protected:
		TES3::MobileProjectile* m_Projectile;
		TES3::Reference* m_TargetReference;
		NI::Vector3 m_CollisionPoint;
		NI::Vector3 m_Position;
		NI::Vector3 m_Velocity;
	};
}