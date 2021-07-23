#include "LuaReferenceDeactivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ReferenceDeactivatedEvent::ReferenceDeactivatedEvent(TES3::Reference* reference) :
				ObjectFilteredEvent("referenceDeactivated", reference),
				m_Reference(reference)
			{

			}

			sol::table ReferenceDeactivatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Reference;

				return eventData;
			}

			bool ReferenceDeactivatedEvent::m_EventEnabled = false;
		}
	}
}
