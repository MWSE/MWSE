#include "LuaMenuStateEvent.h"

#include "LuaManager.h"

#include "TES3UIElement.h"
#include "TES3UIManager.h"

namespace mwse::lua::event {
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
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["menuMode"] = m_InMenuMode;
		if (m_InMenuMode) {
			eventData["menu"] = TES3::UI::getMenuOnTop();
		}

		return eventData;
	}

	sol::object MenuStateEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		auto menu = TES3::UI::getMenuOnTop();
		if (menu) {
			options["filter"] = menu->name.cString;
		}

		return options;
	}
}
