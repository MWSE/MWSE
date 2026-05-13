#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "NIPoint3.h"

namespace mwse::lua::event {
	class MobileProjectileObjectCollisionEvent : public ObjectFilteredEvent, public DisableableEvent<MobileProjectileObjectCollisionEvent> {
	public:
		MobileProjectileObjectCollisionEvent(TES3::MobileProjectile* projectile, TES3::Reference* targetReference, NI::Point3& point, NI::Point3& pos, NI::Point3& vel);
		sol::table createEventTable();

	protected:
		TES3::MobileProjectile* m_Projectile;
		TES3::Reference* m_TargetReference;
		NI::Point3 m_CollisionPoint;
		NI::Point3 m_Position;
		NI::Point3 m_Velocity;
	};
}