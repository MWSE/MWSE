#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "NIPoint3.h"

namespace mwse::lua::event {
	class MobileProjectileTerrainCollisionEvent : public ObjectFilteredEvent, public DisableableEvent<MobileProjectileTerrainCollisionEvent> {
	public:
		MobileProjectileTerrainCollisionEvent(TES3::MobileProjectile* projectile, NI::Point3& point, NI::Point3& pos, NI::Point3& vel);
		sol::table createEventTable();

	protected:
		TES3::MobileProjectile* m_Projectile;
		NI::Point3 m_CollisionPoint;
		NI::Point3 m_Position;
		NI::Point3 m_Velocity;
	};
}