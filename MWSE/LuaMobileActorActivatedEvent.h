#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class MobileActorActivatedEvent : public ObjectFilteredEvent, public DisableableEvent<MobileActorActivatedEvent> {
			public:
				MobileActorActivatedEvent(TES3::MobileObject * mobile);
				sol::table createEventTable();

			protected:
				TES3::MobileObject* m_Mobile;
			};
		}
	}
}
