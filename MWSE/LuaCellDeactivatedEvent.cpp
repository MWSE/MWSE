#include "LuaCellDeactivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"

namespace mwse {
	namespace lua {
		namespace event {
			CellDeactivatedEvent::CellDeactivatedEvent(TES3::Cell* cell) :
				ObjectFilteredEvent("cellDeactivated", cell),
				m_Cell(cell)
			{

			}

			sol::table CellDeactivatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["cell"] = m_Cell;

				return eventData;
			}

			bool CellDeactivatedEvent::m_EventEnabled = false;
		}
	}
}
