#include "LuaSimulateEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	SimulateEvent::SimulateEvent(float delta, double timestamp) :
		GenericEvent("simulate"),
		m_Delta(delta),
		m_Timestamp(timestamp)
	{

	}

	sol::table SimulateEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["menuMode"] = false;
		eventData["delta"] = m_Delta;
		eventData["timestamp"] = m_Timestamp;

		return eventData;
	}
}
