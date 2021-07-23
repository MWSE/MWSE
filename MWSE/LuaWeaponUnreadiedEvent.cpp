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
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Reference;

				return eventData;
			}

			bool WeaponUnreadiedEvent::m_EventEnabled = false;
		}
	}
}
