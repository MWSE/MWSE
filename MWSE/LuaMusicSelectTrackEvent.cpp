#include "LuaMusicSelectTrackEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	MusicSelectTrackEvent::MusicSelectTrackEvent(int situation) :
		GenericEvent("musicSelectTrack"),
		m_Situation(situation)
	{

	}

	sol::table MusicSelectTrackEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();
		eventData["situation"] = m_Situation;
		return eventData;
	}
}
