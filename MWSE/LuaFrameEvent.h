#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			// Enter frame event.
			class FrameEvent : public GenericEvent, public DisableableEvent<FrameEvent> {
			public:
				FrameEvent(float delta, bool menuMode, double timestamp);
				sol::table createEventTable();

			protected:
				bool m_MenuMode;
				float m_Delta;
				double m_Timestamp;
			};
		}
	}
}
