#include "LuaLeveledCreaturePickedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"
#include "TES3LeveledList.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	LeveledCreaturePickedEvent::LeveledCreaturePickedEvent(TES3::LeveledCreature * list, TES3::Object * vanillaResult) :
		ObjectFilteredEvent("leveledCreaturePicked", list),
		m_List(list),
		m_Result(vanillaResult)
	{

	}

	sol::table LeveledCreaturePickedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;
		sol::table eventData = state.create_table();

		eventData["list"] = m_List;
		eventData["pick"] = m_Result;

		// Try to determine what the spawning source is.
		if (m_LastLeveledSourceReference) {
			eventData["source"] = "reference";
			eventData["spawner"] = m_LastLeveledSourceReference;
			eventData["cell"] = m_LastLeveledSourceReference->getCell();
		}
		else if (m_IsForEmptyCell) {
			eventData["source"] = "emptyCell";
			eventData["cell"] = TES3::DataHandler::get()->nonDynamicData->cells->tail->data;
		}
		else if (auto macp = TES3::WorldController::get() ? TES3::WorldController::get()->getMobilePlayer() : nullptr; macp && macp->sleeping) {
			eventData["source"] = "sleepInterrupt";
			eventData["cell"] = TES3::DataHandler::get()->currentCell;
		}
		else {
			eventData["source"] = "script";
		}

		return eventData;
	}

	bool LeveledCreaturePickedEvent::m_EventEnabled = false;
	TES3::Reference* LeveledCreaturePickedEvent::m_LastLeveledSourceReference = nullptr;
	bool LeveledCreaturePickedEvent::m_IsForEmptyCell = false;
}
