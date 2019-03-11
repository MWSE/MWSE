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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["actor"] = makeLuaObject(m_MobileActor);

				return eventData;
			}
		}
	}
}
