#include "LuaGamepadPovEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	GamepadPovEvent::GamepadPovEvent(unsigned int gamepad, unsigned int pov, DWORD value, DWORD previousValue) :
		GenericEvent("gamepadPov"),
		m_Gamepad(gamepad),
		m_Pov(pov),
		m_IsCentered(LOWORD(value) == std::numeric_limits<WORD>::max()),
		m_WasCentered(LOWORD(previousValue) == std::numeric_limits<WORD>::max()),
		m_Value(m_IsCentered ? 0.0f : value / 100.0f),
		m_PreviousValue(m_WasCentered ? 0.0f : previousValue / 100.0f)
	{
	}

	sol::table GamepadPovEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto eventData = stateHandle.getState().create_table();
		eventData["gamepad"] = m_Gamepad;
		eventData["pov"] = m_Pov;
		eventData["isCentered"] = m_IsCentered;
		eventData["wasCentered"] = m_WasCentered;
		eventData["value"] = m_Value;
		eventData["previousValue"] = m_PreviousValue;
		return eventData;
	}

	sol::object GamepadPovEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto options = stateHandle.getState().create_table();
		options["filter"] = m_Gamepad;
		return options;
	}
}
