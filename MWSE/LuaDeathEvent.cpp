#include "LuaDeathEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			DeathEvent::DeathEvent(TES3::MobileActor* mobileActor) :
				ObjectFilteredEvent("death", mobileActor->reference),
				m_MobileActor(mobileActor)
			{

			}

			sol::table DeathEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_MobileActor;
				eventData["reference"] = m_MobileActor->reference;

				return eventData;
			}

			bool DeathEvent::m_EventEnabled = false;
		}
	}
}
