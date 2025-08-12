#include "LuaSavedGameEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	SavedGameEvent::SavedGameEvent(const char* saveName, const char* fileName) :
		GenericEvent("saved"),
		m_SaveName(saveName),
		m_FileName(fileName)
	{

	}

	sol::table SavedGameEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["name"] = m_SaveName;
		eventData["filename"] = m_FileName;

		return eventData;
	}

	sol::object SavedGameEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_FileName;

		return options;
	}
}
