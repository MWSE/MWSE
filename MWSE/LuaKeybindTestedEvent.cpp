#include "LuaKeybindTestedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	KeybindTestedEvent::KeybindTestedEvent(unsigned int keybind, unsigned int transition, bool result) :
		GenericEvent("keybindTested"),
		m_Keybind(keybind),
		m_Transition(transition),
		m_Result(result)
	{

	}

	sol::table KeybindTestedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["keybind"] = m_Keybind;
		eventData["transition"] = m_Transition;
		eventData["result"] = m_Result;

		return eventData;
	}

	sol::object KeybindTestedEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_Keybind;

		return options;
	}
}
