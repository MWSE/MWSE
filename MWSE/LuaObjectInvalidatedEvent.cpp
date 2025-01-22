#include "LuaObjectInvalidatedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	ObjectInvalidatedEvent::ObjectInvalidatedEvent(sol::object object) :
		GenericEvent("objectInvalidated"),
		m_Object(object)
	{

	}

	sol::table ObjectInvalidatedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["object"] = m_Object;

		return eventData;
	}
}
