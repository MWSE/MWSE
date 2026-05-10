#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "NIPoint3.h"

namespace mwse::lua::event {
	class JumpEvent : public ObjectFilteredEvent, public DisableableEvent<JumpEvent> {
	public:
		JumpEvent(TES3::MobileActor* mobile, NI::Point3& velocity, bool applyFatigueCost, bool isDefaultJump);
		sol::table createEventTable();

	protected:
		TES3::MobileActor* m_MobileActor;
		NI::Point3 m_Velocity;
		bool m_ApplyFatigueCost;
		bool m_IsDefaultJump;
	};
}
