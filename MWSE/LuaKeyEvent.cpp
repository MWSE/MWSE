#include "LuaKeyEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			KeyEvent::KeyEvent(int keyCode, bool pressed, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
				GenericEvent("key"),
				m_KeyCode(keyCode),
				m_Pressed(pressed),
				m_ControlDown(controlDown),
				m_ShiftDown(shiftDown),
				m_AltDown(altDown),
				m_SuperDown(superDown)
			{

			}

			sol::table KeyEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["keyCode"] = m_KeyCode;
				eventData["pressed"] = m_Pressed;
				eventData["isControlDown"] = m_ControlDown;
				eventData["isShiftDown"] = m_ShiftDown;
				eventData["isAltDown"] = m_AltDown;
				eventData["isSuperDown"] = m_SuperDown;

				return eventData;
			}

			sol::object KeyEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = m_KeyCode;

				return options;
			}
		}
	}
}
