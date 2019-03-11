#include "LuaFrameEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			FrameEvent::FrameEvent(float delta, bool menuMode) :
				GenericEvent("enterFrame"),
				m_Delta(delta),
				m_MenuMode(menuMode)
			{

			}

			sol::table FrameEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["menuMode"] = m_MenuMode;
				eventData["delta"] = m_Delta;

				return eventData;
			}
		}
	}
}
