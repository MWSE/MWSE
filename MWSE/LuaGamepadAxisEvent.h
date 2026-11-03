#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class GamepadAxisEvent : public GenericEvent, public DisableableEvent<GamepadAxisEvent> {
	public:
		GamepadAxisEvent(unsigned int gamepad, unsigned int axis, LONG value, LONG previousValue);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		unsigned int m_Gamepad;
		unsigned int m_Axis;
		LONG m_Value;
		LONG m_PreviousValue;
	};
}
