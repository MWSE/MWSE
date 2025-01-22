#include "LuaCalcSpellmakingSpellPointCostEvent.h"

#include "LuaManager.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalculateSpellmakingSpellPointCostEvent::CalculateSpellmakingSpellPointCostEvent(TES3::MobileActor* serviceActor, float spellPointCost) :
		ObjectFilteredEvent("calcSpellmakingSpellPointCost", serviceActor->reference),
		m_ServiceActor(serviceActor),
		m_SpellPointCost(spellPointCost)
	{

	}

	sol::table CalculateSpellmakingSpellPointCostEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_ServiceActor;
		if (m_ServiceActor) {
			eventData["reference"] = m_ServiceActor->reference;
		}

		eventData["spellPointCost"] = m_SpellPointCost;

		return eventData;
	}
}
