#include "LuaAddTopicEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Dialogue.h"

namespace mwse {
	namespace lua {
		namespace event {
			AddTopicEvent::AddTopicEvent(TES3::Dialogue * topic) :
				GenericEvent("topicAdded"),
				m_Topic(topic)
			{

			}

			sol::table AddTopicEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["topic"] = m_Topic;

				return eventData;
			}

			bool AddTopicEvent::m_EventEnabled = false;
		}
	}
}
