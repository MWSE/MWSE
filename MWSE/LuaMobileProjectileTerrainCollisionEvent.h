#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "NIVector3.h"

namespace mwse::lua::event {
	class MobileProjectileTerrainCollisionEvent : public ObjectFilteredEvent, public DisableableEvent<MobileProjectileTerrainCollisionEvent> {
	public:
		MobileProjectileTerrainCollisionEvent(TES3::MobileProjectile* projectile, NI::Vector3& point, NI::Vector3& pos, NI::Vector3& vel);
		sol::table createEventTable();

	protected:
		TES3::MobileProjectile* m_Projectile;
		NI::Vector3 m_CollisionPoint;
		NI::Vector3 m_Position;
		NI::Vector3 m_Velocity;
	};
}