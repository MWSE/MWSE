#include "LuaCombatStartEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CombatStartEvent::CombatStartEvent(TES3::MobileActor* mobileActor, TES3::MobileActor* target) :
		ObjectFilteredEvent("combatStart", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_Target(target)
	{

	}

	sol::table CombatStartEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["actor"] = m_MobileActor;
		eventData["target"] = m_Target;

		return eventData;
	}
}
