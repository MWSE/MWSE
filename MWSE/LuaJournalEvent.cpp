#include "LuaJournalEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Dialogue.h"

namespace mwse {
	namespace lua {
		namespace event {
			JournalEvent::JournalEvent(TES3::Dialogue * topic, int oldIndex, int newIndex) :
				GenericEvent("journal"),
				m_Topic(topic),
				m_OldIndex(oldIndex),
				m_NewIndex(newIndex)
			{

			}

			sol::table JournalEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["topic"] = m_Topic;
				eventData["index"] = m_NewIndex;
				eventData["previousIndex"] = m_OldIndex;
				eventData["new"] = (m_OldIndex == 0 && m_NewIndex > m_OldIndex);

				return eventData;
			}

			bool JournalEvent::m_EventEnabled = false;
		}
	}
}
