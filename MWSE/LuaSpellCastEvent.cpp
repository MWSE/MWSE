#include "LuaSpellCastEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse::lua::event {
	SpellCastEvent::SpellCastEvent(TES3::Spell* spell, TES3::MobileActor* caster, float castChance, int weakestSchool) :
		GenericEvent("spellCast"),
		m_Spell(spell),
		m_Caster(caster),
		m_CastChance(castChance),
		m_WeakestSchool(weakestSchool)
	{

	}

	sol::table SpellCastEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["caster"] = m_Caster->reference;
		eventData["source"] = m_Spell;
		eventData["castChance"] = m_CastChance;
		eventData["weakestSchool"] = m_WeakestSchool;

		return eventData;
	}

	sol::object SpellCastEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_Spell;

		return options;
	}
}
