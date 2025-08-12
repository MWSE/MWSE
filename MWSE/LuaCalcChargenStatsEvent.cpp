#include "LuaCalcChargenStatsEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	CalcChargenStatsEvent::CalcChargenStatsEvent() :
		GenericEvent("calcChargenStats")
	{
	}

	sol::table CalcChargenStatsEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		return eventData;
	}
}
