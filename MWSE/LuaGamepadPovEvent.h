#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class GamepadPovEvent : public GenericEvent, public DisableableEvent<GamepadPovEvent> {
	public:
		GamepadPovEvent(unsigned int gamepad, unsigned int pov, DWORD value, DWORD previousValue);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		unsigned int m_Gamepad;
		unsigned int m_Pov;
		bool m_IsCentered;
		bool m_WasCentered;
		float m_Value;
		float m_PreviousValue;
	};
}
