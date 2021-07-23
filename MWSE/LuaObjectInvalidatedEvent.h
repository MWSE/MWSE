#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class ObjectInvalidatedEvent : public GenericEvent, public DisableableEvent<ObjectInvalidatedEvent> {
			public:
				ObjectInvalidatedEvent(sol::object object);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				sol::object m_Object;
			};
		}
	}
}
