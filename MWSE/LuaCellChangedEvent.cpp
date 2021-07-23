#include "LuaCellChangedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"

namespace mwse {
	namespace lua {
		namespace event {
			CellChangedEvent::CellChangedEvent(TES3::Cell* cell, TES3::Cell* previousCell) :
				ObjectFilteredEvent("cellChanged", cell),
				m_Cell(cell),
				m_PreviousCell(previousCell)
			{

			}

			sol::table CellChangedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["cell"] = m_Cell;
				eventData["previousCell"] = m_PreviousCell;

				return eventData;
			}

			bool CellChangedEvent::m_EventEnabled = false;
		}
	}
}
