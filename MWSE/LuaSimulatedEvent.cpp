#include "LuaSimulatedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	SimulatedEvent::SimulatedEvent() :
		GenericEvent("simulated")
	{
	}

	sol::table SimulatedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		return eventData;
	}
}
