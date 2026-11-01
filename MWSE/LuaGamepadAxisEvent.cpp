#include "LuaGamepadAxisEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	GamepadAxisEvent::GamepadAxisEvent(unsigned int gamepad, unsigned int axis, LONG value, LONG previousValue) :
		GenericEvent("gamepadAxis"),
		m_Gamepad(gamepad),
		m_Axis(axis),
		m_Value(value),
		m_PreviousValue(previousValue)
	{
	}

	sol::table GamepadAxisEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto eventData = stateHandle.getState().create_table();
		eventData["gamepad"] = m_Gamepad;
		eventData["axis"] = m_Axis;
		eventData["value"] = m_Value;
		eventData["previousValue"] = m_PreviousValue;
		return eventData;
	}

	sol::object GamepadAxisEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto options = stateHandle.getState().create_table();
		options["filter"] = m_Gamepad;
		return options;
	}
}
