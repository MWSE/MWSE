#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		namespace event {
			class PowerRechargedEvent : public ObjectFilteredEvent, public DisableableEvent<PowerRechargedEvent> {
			public:
				PowerRechargedEvent(TES3::Spell* power, TES3::MobileActor* mobile);
				sol::table createEventTable();

			protected:
				TES3::Spell* m_Power;
				TES3::MobileActor* m_Mobile;
			};
		}
	}
}
