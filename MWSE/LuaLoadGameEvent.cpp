#include "LuaLoadGameEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	LoadGameEvent::LoadGameEvent(const char* fileName, bool quickLoad, bool newGame) :
		GenericEvent("load"),
		m_FileName(fileName),
		m_QuickLoad(quickLoad),
		m_NewGame(newGame)
	{
		if (m_FileName == NULL && !m_NewGame) {
			m_QuickLoad = true;
			m_FileName = "quiksave.ess";
		}
	}

	sol::table LoadGameEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		if (m_NewGame) {
			eventData["newGame"] = true;
			eventData["quickload"] = false;
		}
		else {
			std::string filename = m_FileName;
			eventData["filename"] = filename.substr(0, filename.find_last_of('.'));;
			eventData["quickload"] = m_QuickLoad;
			eventData["newGame"] = false;
		}

		return eventData;
	}

	sol::object LoadGameEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_FileName;

		return options;
	}
}
