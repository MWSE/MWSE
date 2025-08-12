#include "LuaLoadedGameEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	LoadedGameEvent::LoadedGameEvent(const char* fileName, bool quickLoad, bool newGame) :
		GenericEvent("loaded"),
		m_FileName(fileName),
		m_QuickLoad(quickLoad),
		m_NewGame(newGame)
	{
		if (m_FileName == NULL && !m_NewGame) {
			m_QuickLoad = true;
			m_FileName = "quiksave.ess";
		}
	}

	sol::table LoadedGameEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
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

		auto mobilePlayer = TES3::WorldController::get()->getMobilePlayer();
		if (mobilePlayer) {
			eventData["mobile"] = mobilePlayer;
			eventData["reference"] = mobilePlayer->reference;
		}

		return eventData;
	}

	sol::object LoadedGameEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_FileName;

		return options;
	}
}
