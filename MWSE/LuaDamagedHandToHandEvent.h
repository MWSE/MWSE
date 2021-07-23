#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class DamagedHandToHandEvent : public ObjectFilteredEvent, public DisableableEvent<DamagedHandToHandEvent> {
			public:
				DamagedHandToHandEvent(TES3::MobileActor* mobileActor, float fatigueDamage);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_MobileActor;
				float m_FatigueDamage;
			};
		}
	}
}
