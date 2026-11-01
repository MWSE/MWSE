#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class GamepadButtonUpEvent : public GenericEvent, public DisableableEvent<GamepadButtonUpEvent> {
	public:
		GamepadButtonUpEvent(unsigned int gamepad, unsigned int button);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		unsigned int m_Gamepad;
		unsigned int m_Button;
	};
}
