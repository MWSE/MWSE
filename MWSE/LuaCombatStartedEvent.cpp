#include "LuaCombatStartedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CombatStartedEvent::CombatStartedEvent(TES3::MobileActor* mobileActor, TES3::MobileActor* target) :
				ObjectFilteredEvent("combatStarted", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_Target(target)
			{

			}

			sol::table CombatStartedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["actor"] = m_MobileActor;
				eventData["target"] = m_Target;

				return eventData;
			}

			bool CombatStartedEvent::m_EventEnabled = false;
		}
	}
}
