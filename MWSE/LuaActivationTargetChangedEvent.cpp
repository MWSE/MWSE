#include "LuaActivationTargetChangedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ActivationTargetChangedEvent::ActivationTargetChangedEvent(TES3::Reference* previous, TES3::Reference* current) :
				ObjectFilteredEvent("activationTargetChanged", current),
				m_PreviousReference(previous),
				m_CurrentReference(current)
			{

			}

			sol::table ActivationTargetChangedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["previous"] = m_PreviousReference;
				eventData["current"] = m_CurrentReference;

				return eventData;
			}

			bool ActivationTargetChangedEvent::m_EventEnabled = false;
		}
	}
}
