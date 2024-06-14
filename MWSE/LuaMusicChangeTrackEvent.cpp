#include "LuaMusicChangeTrackEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	MusicChangeTrackEvent::MusicChangeTrackEvent(const char* music, float volume, int crossfadeMS, int situation) :
		GenericEvent("musicChangeTrack"),
		m_Music(music),
		m_Volume(volume),
		m_CrossfadeMS(crossfadeMS),
		m_Situation(situation)
	{

	}

	sol::table MusicChangeTrackEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();
		eventData["music"] = m_Music;
		eventData["volume"] = m_Volume;
		eventData["crossfade"] = m_CrossfadeMS;
		eventData["situation"] = (int)m_Situation;
		eventData["context"] = ms_Context;
		return eventData;
	}
	const char* MusicChangeTrackEvent::ms_Context = nullptr;
}
