#include "LuaSpellResistEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Spell.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	SpellResistEvent::SpellResistEvent(TES3::MagicSourceInstance* spellInstance, TES3::MagicEffectInstance* effectInstance, int effectIndex, int resistAttribute) :
		GenericEvent("spellResist"),
		m_MagicSourceInstance(spellInstance),
		m_EffectInstance(effectInstance),
		m_EffectIndex(effectIndex),
		m_ResistAttribute(resistAttribute)
	{

	}

	sol::table SpellResistEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_EffectInstance->target;
		eventData["resistedPercent"] = m_EffectInstance->resistedPercent;

		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["effectIndex"] = m_EffectIndex;
		eventData["effectInstance"] = m_EffectInstance;
		eventData["resistAttribute"] = m_ResistAttribute;

		// Get the specific effect on the source.
		TES3::Effect* effects = m_MagicSourceInstance->sourceCombo.getSourceEffects();
		if (effects) {
			eventData["effect"] = effects[m_EffectIndex];
		}

		// Get cast chance if it's a spell.
		if (m_MagicSourceInstance->sourceCombo.sourceType == TES3::MagicSourceType::Spell || m_MagicSourceInstance->sourceCombo.sourceType == TES3::MagicSourceType::Enchantment) {
			eventData["spellCastChance"] = m_MagicSourceInstance->sourceCombo.source.asSpell->calculateCastChance(m_MagicSourceInstance->caster);
		}

		return eventData;
	}

	sol::object SpellResistEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;

		return options;
	}
}
