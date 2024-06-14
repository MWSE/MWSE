#include "LuaShowRestWaitMenuEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	ShowRestWaitMenuEvent::ShowRestWaitMenuEvent(bool allowRest, bool scripted) :
		GenericEvent("uiShowRestMenu"),
		m_AllowRest(allowRest),
		m_Scripted(scripted)
	{

	}

	sol::table ShowRestWaitMenuEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["allowRest"] = m_AllowRest;
		eventData["scripted"] = m_Scripted;

		return eventData;
	}
}
