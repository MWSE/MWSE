#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class AddSoundEvent : public ObjectFilteredEvent, public DisableableEvent<AddSoundEvent> {
	public:
		AddSoundEvent(TES3::Sound* sound, TES3::Reference* reference, int playbackFlags, unsigned char volume, float pitch, bool isVoiceover);
		sol::table createEventTable();

	protected:
		TES3::Sound* m_Sound;
		TES3::Reference* m_Reference;
		int m_Flags;
		unsigned char m_Volume;
		float m_Pitch;
		bool m_IsVoiceover;
	};
}
