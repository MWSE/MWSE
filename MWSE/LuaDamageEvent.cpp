#include "LuaDamageEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			DamageEvent::DamageEvent(TES3::MobileActor* mobileActor, float damage) :
				ObjectFilteredEvent("damage", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_Damage(damage)
			{

			}

			sol::table DamageEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				eventData["damage"] = m_Damage;

				return eventData;
			}
		}
	}
}
