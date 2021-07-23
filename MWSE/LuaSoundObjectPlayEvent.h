#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class SoundObjectPlayEvent : public ObjectFilteredEvent, public DisableableEvent<SoundObjectPlayEvent> {
			public:
				SoundObjectPlayEvent(TES3::Sound* sound, int playbackFlags, unsigned char volume, float pitch, bool isNot3D);
				sol::table createEventTable();

			protected:
				TES3::Sound* m_Sound;
				int m_PlaybackFlags;
				unsigned char m_Volume;
				float m_Pitch;
				bool m_IsNot3D;

			};
		}
	}
}
