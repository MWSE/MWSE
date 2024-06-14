#include "LuaInfoLinkResolveEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse::lua::event {
	InfoLinkResolveEvent::InfoLinkResolveEvent(const char* topic) :
		GenericEvent("infoLinkResolve"),
		m_Topic(topic)
	{
	}

	sol::table InfoLinkResolveEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["topic"] = m_Topic;
		return eventData;
	}
}
