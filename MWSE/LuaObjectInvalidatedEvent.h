#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class ObjectInvalidatedEvent : public GenericEvent, public DisableableEvent<ObjectInvalidatedEvent> {
	public:
		ObjectInvalidatedEvent(sol::object object);
		sol::table createEventTable();

	protected:
		sol::object m_Object;
	};
}
