#include "LuaMenuStateEvent.h"

#include "LuaManager.h"

#include "UIUtil.h"

#include "TES3UIElement.h"

namespace mwse {
	namespace lua {
		namespace event {
			MenuStateEvent::MenuStateEvent(bool inMenuMode) :
				m_InMenuMode(inMenuMode)
			{

			}

			const char* MenuStateEvent::getEventName() {
				if (m_InMenuMode) {
					return "menuEnter";
				}
				else {
					return "menuExit";
				}
			}

			sol::table MenuStateEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["menuMode"] = m_InMenuMode;
				if (m_InMenuMode) {
					eventData["menu"] = tes3::ui::getTopMenu();
				}

				return eventData;
			}

			sol::object MenuStateEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				auto menu = tes3::ui::getTopMenu();
				if (menu) {
					options["filter"] = menu->name.cString;
				}

				return options;
			}
		}
	}
}
