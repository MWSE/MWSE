#include "LuaAttackStartEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			AttackStartEvent::AttackStartEvent(TES3::MobileActor* mobileActor, TES3::PhysicalAttackType attackType, float attackSpeed) :
				ObjectFilteredEvent("attackStart", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_AttackType(attackType),
				m_AttackSpeed(attackSpeed)
			{

			}

			sol::table AttackStartEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				if (m_MobileActor) {
					eventData["mobile"] = m_MobileActor;
					eventData["reference"] = m_MobileActor->reference;
				}

				eventData["attackType"] = m_AttackType;
				eventData["attackSpeed"] = m_AttackSpeed;

				return eventData;
			}

			bool AttackStartEvent::m_EventEnabled = false;
		}
	}
}
