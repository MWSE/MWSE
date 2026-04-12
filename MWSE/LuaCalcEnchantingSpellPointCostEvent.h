#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class CalculateEnchantingSpellPointCostEvent : public ObjectFilteredEvent, public DisableableEvent<CalculateEnchantingSpellPointCostEvent> {
	public:
		CalculateEnchantingSpellPointCostEvent(TES3::MobileActor* serviceActor, float spellPointCost);
		sol::table createEventTable();

	protected:
		TES3::MobileActor* m_ServiceActor;
		float m_SpellPointCost;
	};
}
