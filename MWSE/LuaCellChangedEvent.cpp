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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["cell"] = makeLuaObject(m_Cell);
				eventData["previousCell"] = makeLuaObject(m_PreviousCell);

				return eventData;
			}
		}
	}
}
