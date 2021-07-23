#include "LuaObjectInvalidatedEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			ObjectInvalidatedEvent::ObjectInvalidatedEvent(sol::object object) :
				GenericEvent("objectInvalidated"),
				m_Object(object)
			{
				
			}

			sol::table ObjectInvalidatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["object"] = m_Object;

				return eventData;
			}

			sol::object ObjectInvalidatedEvent::getEventOptions() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["filter"] = m_Object;

				return eventData;
			}

			bool ObjectInvalidatedEvent::m_EventEnabled = false;
		}
	}
}
