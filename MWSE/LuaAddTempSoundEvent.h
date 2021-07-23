#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class AddTempSoundEvent : public GenericEvent, public DisableableEvent<AddTempSoundEvent> {
	public:
		AddTempSoundEvent(const char* path, TES3::Reference* reference, int playbackFlags, unsigned char volume, float pitch, bool isVoiceover, TES3::Sound* sound);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		const char* m_Path;
		TES3::Reference* m_Reference;
		int m_Flags;
		unsigned char m_Volume;
		float m_Pitch;
		bool m_IsVoiceover;
		TES3::Sound* m_Sound;
	};
}
