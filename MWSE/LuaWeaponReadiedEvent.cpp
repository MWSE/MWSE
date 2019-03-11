#include "LuaWeaponReadiedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			WeaponReadiedEvent::WeaponReadiedEvent(TES3::Reference* reference) :
				ObjectFilteredEvent("weaponReadied", reference),
				m_Reference(reference)
			{

			}

			sol::table WeaponReadiedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["reference"] = makeLuaObject(m_Reference);
				eventData["weaponStack"] = m_Reference->getAttachedMobileActor()->readiedWeapon;

				return eventData;
			}
		}
	}
}
