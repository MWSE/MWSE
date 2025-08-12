#include "LuaCombatStopEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CombatStopEvent::CombatStopEvent(TES3::MobileActor* mobileActor) :
		ObjectFilteredEvent("combatStop", mobileActor->reference),
		m_MobileActor(mobileActor)
	{

	}

	sol::table CombatStopEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["actor"] = m_MobileActor;

		return eventData;
	}
}
