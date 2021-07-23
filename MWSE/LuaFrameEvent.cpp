#include "LuaFrameEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			FrameEvent::FrameEvent(float delta, bool menuMode, double timestamp) :
				GenericEvent("enterFrame"),
				m_Delta(delta),
				m_MenuMode(menuMode),
				m_Timestamp(timestamp)
			{

			}

			sol::table FrameEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["menuMode"] = m_MenuMode;
				eventData["delta"] = m_Delta;
				eventData["timestamp"] = m_Timestamp;

				return eventData;
			}

			bool FrameEvent::m_EventEnabled = false;
		}
	}
}
