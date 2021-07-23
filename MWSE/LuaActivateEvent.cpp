#include "LuaActivateEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ActivateEvent::ActivateEvent(TES3::Reference* activator, TES3::Reference* target) :
				ObjectFilteredEvent("activate", target),
				m_Activator(activator),
				m_Target(target)
			{

			}

			sol::table ActivateEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["activator"] = m_Activator;
				eventData["target"] = m_Target;

				return eventData;
			}

			bool ActivateEvent::m_EventEnabled = false;
		}
	}
}
