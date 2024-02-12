#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class ObjectCreatedEvent : public ObjectFilteredEvent, public DisableableEvent<ObjectCreatedEvent> {
	public:
		ObjectCreatedEvent(TES3::BaseObject* object);
		sol::table createEventTable();

	protected:
		TES3::BaseObject* m_Object;
	};
}
