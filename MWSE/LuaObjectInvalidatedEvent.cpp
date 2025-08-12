#include "LuaObjectInvalidatedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	ObjectInvalidatedEvent::ObjectInvalidatedEvent(sol::object object) :
		GenericEvent("objectInvalidated"),
		m_Object(object)
	{

	}

	sol::table ObjectInvalidatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["object"] = m_Object;

		return eventData;
	}
}
