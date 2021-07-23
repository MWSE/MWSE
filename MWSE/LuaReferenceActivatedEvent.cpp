#include "LuaReferenceActivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ReferenceActivatedEvent::ReferenceActivatedEvent(TES3::Reference* reference) :
				ObjectFilteredEvent("referenceActivated", reference),
				m_Reference(reference)
			{

			}

			sol::table ReferenceActivatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Reference;

				return eventData;
			}

			bool ReferenceActivatedEvent::m_EventEnabled = false;
		}
	}
}
