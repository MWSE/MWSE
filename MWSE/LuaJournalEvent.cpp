#include "LuaJournalEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"

namespace mwse::lua::event {
	JournalEvent::JournalEvent(TES3::Dialogue* topic, int oldIndex, int newIndex) :
		GenericEvent("journal"),
		m_Topic(topic),
		m_OldIndex(oldIndex),
		m_NewIndex(newIndex)
	{

	}

	sol::table JournalEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["topic"] = m_Topic;
		eventData["index"] = m_NewIndex;
		eventData["previousIndex"] = m_OldIndex;
		eventData["new"] = (m_OldIndex == 0 && m_NewIndex > m_OldIndex);
		eventData["info"] = m_Topic->getJournalInfoForIndex(m_NewIndex);
		eventData["previousInfo"] = m_Topic->getJournalInfoForIndex(m_OldIndex);

		return eventData;
	}
}
