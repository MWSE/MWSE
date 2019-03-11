#include "LuaMouseWheelEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			MouseWheelEvent::MouseWheelEvent(int delta, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
				GenericEvent("mouseWheel"),
				m_Delta(delta),
				m_ControlDown(controlDown),
				m_ShiftDown(shiftDown),
				m_AltDown(altDown),
				m_SuperDown(superDown)
			{

			}

			sol::table MouseWheelEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["delta"] = m_Delta;
				eventData["isControlDown"] = m_ControlDown;
				eventData["isShiftDown"] = m_ShiftDown;
				eventData["isAltDown"] = m_AltDown;
				eventData["isSuperDown"] = m_SuperDown;

				return eventData;
			}
		}
	}
}
