#include "LuaShowRestWaitMenuEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			ShowRestWaitMenuEvent::ShowRestWaitMenuEvent(bool allowRest, bool scripted) :
				GenericEvent("uiShowRestMenu"),
				m_AllowRest(allowRest),
				m_Scripted(scripted)
			{

			}

			sol::table ShowRestWaitMenuEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["allowRest"] = m_AllowRest;
				eventData["scripted"] = m_Scripted;

				return eventData;
			}
		}
	}
}
