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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				eventData["reference"] = makeLuaObject(m_MobileActor->reference);

				return eventData;
			}
		}
	}
}
