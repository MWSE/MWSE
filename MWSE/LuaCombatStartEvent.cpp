#include "LuaCombatStartEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CombatStartEvent::CombatStartEvent(TES3::MobileActor* mobileActor, TES3::MobileActor* target) :
				ObjectFilteredEvent("combatStart", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_Target(target)
			{

			}

			sol::table CombatStartEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["actor"] = makeLuaObject(m_MobileActor);
				eventData["target"] = makeLuaObject(m_Target);

				return eventData;
			}
		}
	}
}
