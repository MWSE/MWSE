#include "LuaFrameEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	FrameEvent::FrameEvent(float delta, bool menuMode, double timestamp) :
		GenericEvent("enterFrame"),
		m_Delta(delta),
		m_MenuMode(menuMode),
		m_Timestamp(timestamp)
	{

	}

	sol::table FrameEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["menuMode"] = m_MenuMode;
		eventData["delta"] = m_Delta;
		eventData["timestamp"] = m_Timestamp;

		return eventData;
	}
}
