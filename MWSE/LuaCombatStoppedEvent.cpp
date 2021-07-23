#include "LuaCombatStoppedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CombatStoppedEvent::CombatStoppedEvent(TES3::MobileActor* mobileActor) :
				ObjectFilteredEvent("combatStopped", mobileActor->reference),
				m_MobileActor(mobileActor)
			{

			}

			sol::table CombatStoppedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["actor"] = m_MobileActor;

				return eventData;
			}

			bool CombatStoppedEvent::m_EventEnabled = false;
		}
	}
}
