#include "LuaRestInterruptEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3LeveledList.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	RestInterruptEvent::RestInterruptEvent(TES3::LeveledCreature* leveledCreature) :
		GenericEvent("restInterrupt"),
		m_Creature(leveledCreature)
	{

	}

	sol::table RestInterruptEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["creature"] = m_Creature;

		// Add easy access to sleeping and waiting state.
		TES3::MobilePlayer* macp = TES3::WorldController::get()->getMobilePlayer();
		eventData["resting"] = macp->sleeping;
		eventData["waiting"] = macp->waiting;

		return eventData;
	}
}
