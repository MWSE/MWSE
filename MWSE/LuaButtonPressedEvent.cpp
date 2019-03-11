#include "LuaButtonPressedEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			ButtonPressedEvent::ButtonPressedEvent(int buttonId) :
				GenericEvent("buttonPressed"),
				m_ButtonId(buttonId)
			{

			}

			sol::table ButtonPressedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["button"] = m_ButtonId;

				return eventData;
			}
		}
	}
}
