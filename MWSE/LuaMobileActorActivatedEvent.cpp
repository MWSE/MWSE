#include "LuaMobileActorActivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileActorActivatedEvent::MobileActorActivatedEvent(TES3::MobileObject* mobile) :
				ObjectFilteredEvent("mobileActivated", mobile->reference),
				m_Mobile(mobile)
			{

			}

			sol::table MobileActorActivatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Mobile->reference;
				eventData["mobile"] = m_Mobile;

				return eventData;
			}

			bool MobileActorActivatedEvent::m_EventEnabled = false;
		}
	}
}
