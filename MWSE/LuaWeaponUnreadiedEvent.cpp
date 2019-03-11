#include "LuaWeaponUnreadiedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			WeaponUnreadiedEvent::WeaponUnreadiedEvent(TES3::Reference* reference) :
				ObjectFilteredEvent("weaponUnreadied", reference),
				m_Reference(reference)
			{

			}

			sol::table WeaponUnreadiedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["reference"] = makeLuaObject(m_Reference);

				return eventData;
			}
		}
	}
}
