#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "NIPoint3.h"

namespace mwse::lua::event {
	class MobileSpellProjectileTerrainCollisionEvent : public ObjectFilteredEvent, public DisableableEvent<MobileSpellProjectileTerrainCollisionEvent> {
	public:
		MobileSpellProjectileTerrainCollisionEvent(TES3::MobileSpellProjectile* projectile, NI::Point3& point, NI::Point3& pos, NI::Point3& vel);
		sol::table createEventTable();

	protected:
		TES3::MobileSpellProjectile* m_Projectile;
		NI::Point3 m_CollisionPoint;
		NI::Point3 m_Position;
		NI::Point3 m_Velocity;
	};
}
