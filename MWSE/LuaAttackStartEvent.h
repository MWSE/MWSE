#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3MobileActor.h"

namespace mwse {
	namespace lua {
		namespace event {
			class AttackStartEvent : public ObjectFilteredEvent, public DisableableEvent<AttackStartEvent> {
			public:
				AttackStartEvent(TES3::MobileActor* mobileActor, TES3::PhysicalAttackType attackType, float attackSpeed);
				sol::table createEventTable();

			protected:
				TES3::MobileActor * m_MobileActor;
				TES3::PhysicalAttackType m_AttackType;
				float m_AttackSpeed;
			};
		}
	}
}
