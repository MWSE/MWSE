#include "LuaAddSoundEvent.h"

#include "TES3Reference.h"
#include "TES3Sound.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	AddSoundEvent::AddSoundEvent(TES3::Sound* sound, TES3::Reference* reference, int playbackFlags, unsigned char volume, float pitch, bool isVoiceover) :
		ObjectFilteredEvent("addSound", sound),
		m_Sound(sound),
		m_Reference(reference),
		m_Flags(playbackFlags),
		m_Volume(volume),
		m_Pitch(pitch),
		m_IsVoiceover(isVoiceover)
	{

	}

	sol::table AddSoundEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["sound"] = m_Sound;
		eventData["reference"] = m_Reference;
		eventData["flags"] = m_Flags;
		eventData["volume"] = m_Volume;
		eventData["pitch"] = m_Pitch;
		eventData["isVoiceover"] = m_IsVoiceover;

		return eventData;
	}
}
