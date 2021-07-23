#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class InfoLinkResolveEvent : public GenericEvent, public DisableableEvent<InfoLinkResolveEvent> {
			public:
				InfoLinkResolveEvent(const char* topic);
				sol::table createEventTable();

			protected:
				const char* m_Topic;
			};
		}
	}
}
