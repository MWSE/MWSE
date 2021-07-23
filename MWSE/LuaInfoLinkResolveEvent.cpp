#include "LuaInfoLinkResolveEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse {
	namespace lua {
		namespace event {
			InfoLinkResolveEvent::InfoLinkResolveEvent(const char* topic) :
				GenericEvent("infoLinkResolve"),
				m_Topic(topic)
			{
			}

			sol::table InfoLinkResolveEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["topic"] = m_Topic;
				return eventData;
			}

			bool InfoLinkResolveEvent::m_EventEnabled = false;
		}
	}
}
