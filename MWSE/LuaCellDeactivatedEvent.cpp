#include "LuaCellDeactivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"

namespace mwse::lua::event {
	CellDeactivatedEvent::CellDeactivatedEvent(TES3::Cell* cell) :
		ObjectFilteredEvent("cellDeactivated", cell),
		m_Cell(cell)
	{

	}

	sol::table CellDeactivatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["cell"] = m_Cell;

		return eventData;
	}
}
