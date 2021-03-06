#pragma once

#include "LuaGenericEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			GenericEvent::GenericEvent(const char* name) :
				m_EventName(name)
			{

			}

			const char* GenericEvent::getEventName() {
				return m_EventName;
			}
		}
	}
}
