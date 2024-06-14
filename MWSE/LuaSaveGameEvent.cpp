#include "LuaSaveGameEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	SaveGameEvent::SaveGameEvent(const char* saveName, const char* fileName) :
		GenericEvent("save"),
		m_SaveName(saveName),
		m_FileName(fileName)
	{

	}

	sol::table SaveGameEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["name"] = m_SaveName;
		eventData["filename"] = m_FileName;

		return eventData;
	}

	sol::object SaveGameEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_FileName;

		return options;
	}
}
