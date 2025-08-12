#include "LuaButtonPressedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	ButtonPressedEvent::ButtonPressedEvent(int buttonId) :
		GenericEvent("buttonPressed"),
		m_ButtonId(buttonId)
	{

	}

	sol::table ButtonPressedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["button"] = m_ButtonId;

		return eventData;
	}
}
