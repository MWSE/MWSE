#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class GamepadButtonDownEvent : public GenericEvent, public DisableableEvent<GamepadButtonDownEvent> {
	public:
		GamepadButtonDownEvent(unsigned int gamepad, unsigned int button);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		unsigned int m_Gamepad;
		unsigned int m_Button;
	};
}
