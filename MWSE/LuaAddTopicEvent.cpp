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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["topic"] = makeLuaObject(m_Topic);

				return eventData;
			}
		}
	}
}
