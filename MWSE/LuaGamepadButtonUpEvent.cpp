#include "LuaGamepadButtonUpEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	GamepadButtonUpEvent::GamepadButtonUpEvent(unsigned int gamepad, unsigned int button) :
		GenericEvent("gamepadButtonUp"),
		m_Gamepad(gamepad),
		m_Button(button)
	{
	}

	sol::table GamepadButtonUpEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto eventData = stateHandle.getState().create_table();
		eventData["gamepad"] = m_Gamepad;
		eventData["button"] = m_Button;
		return eventData;
	}

	sol::object GamepadButtonUpEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto options = stateHandle.getState().create_table();
		options["filter"] = m_Gamepad;
		return options;
	}
}
