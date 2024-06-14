#include "LuaSoundObjectPlayEvent.h"

#include "LuaManager.h"

#include "TES3Sound.h"

namespace mwse::lua::event {
	SoundObjectPlayEvent::SoundObjectPlayEvent(TES3::Sound* sound, int playbackFlags, unsigned char volume, float pitch, bool isNot3D) :
		ObjectFilteredEvent("soundObjectPlay", sound),
		m_Sound(sound),
		m_PlaybackFlags(playbackFlags),
		m_Volume(volume),
		m_Pitch(pitch),
		m_IsNot3D(isNot3D)
	{

	}

	sol::table SoundObjectPlayEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["sound"] = m_Sound;
		eventData["flags"] = m_PlaybackFlags;
		eventData["volume"] = m_Volume;
		eventData["pitch"] = m_Pitch;
		eventData["isNot3D"] = m_IsNot3D;

		return eventData;
	}
}
