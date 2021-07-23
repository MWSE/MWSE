#include "LuaMobileActorDeactivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileActorDeactivatedEvent::MobileActorDeactivatedEvent(TES3::Reference * reference) :
				ObjectFilteredEvent("mobileDeactivated", reference),
				m_Reference(reference)
			{

			}

			sol::table MobileActorDeactivatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Reference;

				return eventData;
			}

			bool MobileActorDeactivatedEvent::m_EventEnabled = false;
		}
	}
}
