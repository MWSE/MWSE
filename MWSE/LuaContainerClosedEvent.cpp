#include "LuaContainerClosedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ContainerClosedEvent::ContainerClosedEvent(TES3::Reference* reference) :
				ObjectFilteredEvent("containerClosed", reference->baseObject),
				m_Reference(reference)
			{

			}

			sol::table ContainerClosedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Reference;

				return eventData;
			}

			bool ContainerClosedEvent::m_EventEnabled = false;
		}
	}
}
