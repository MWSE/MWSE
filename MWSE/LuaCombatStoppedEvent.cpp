#include "LuaCombatStoppedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CombatStoppedEvent::CombatStoppedEvent(TES3::MobileActor* mobileActor) :
		ObjectFilteredEvent("combatStopped", mobileActor->reference),
		m_MobileActor(mobileActor)
	{

	}

	sol::table CombatStoppedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["actor"] = m_MobileActor;

		return eventData;
	}
}
