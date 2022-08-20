#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class MobileObjectActivatedEvent : public ObjectFilteredEvent, public DisableableEvent<MobileObjectActivatedEvent> {
	public:
		MobileObjectActivatedEvent(TES3::MobileObject* mobile);
		sol::table createEventTable();

	protected:
		TES3::MobileObject* m_Mobile;
	};
}
