#include "LuaCellChangedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"

namespace mwse::lua::event {
	CellChangedEvent::CellChangedEvent(TES3::Cell* cell, TES3::Cell* previousCell) :
		ObjectFilteredEvent("cellChanged", cell),
		m_Cell(cell),
		m_PreviousCell(previousCell)
	{

	}

	sol::table CellChangedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["cell"] = m_Cell;
		eventData["previousCell"] = m_PreviousCell;

		return eventData;
	}
}
