#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class PreventRestEvent : public ObjectFilteredEvent, public DisableableEvent<PreventRestEvent> {
			public:
				PreventRestEvent(TES3::MobileActor* mobile);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_Mobile;
			};
		}
	}
}
