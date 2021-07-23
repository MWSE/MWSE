#include "LuaCombatStopEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CombatStopEvent::CombatStopEvent(TES3::MobileActor* mobileActor) :
				ObjectFilteredEvent("combatStop", mobileActor->reference),
				m_MobileActor(mobileActor)
			{

			}

			sol::table CombatStopEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["actor"] = m_MobileActor;

				return eventData;
			}

			bool CombatStopEvent::m_EventEnabled = false;
		}
	}
}
