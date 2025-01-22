#include "LuaSpellResistedEvent.h"

#include "LuaManager.h"

#include "TES3Reference.h"
#include "TES3MobileActor.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MagicEffectInstance.h"

namespace mwse::lua::event {
	SpellResistedEvent::SpellResistedEvent(TES3::MagicSourceInstance* magicSourceInstance, TES3::MagicEffectInstance* magicEffectInstance, int effectIndex) :
		ObjectFilteredEvent("spellResisted", magicEffectInstance->target),
		m_MagicSourceInstance(magicSourceInstance),
		m_MagicEffectInstance(magicEffectInstance),
		m_EffectIndex(effectIndex)
	{

	}

	sol::table SpellResistedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_MagicEffectInstance->target;
		eventData["mobile"] = m_MagicEffectInstance->target->getAttachedMobileActor();
		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_MagicSourceInstance->target;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["effectIndex"] = m_EffectIndex;

		// Get the specific effect on the source.
		TES3::Effect* effects = m_MagicSourceInstance->sourceCombo.getSourceEffects();
		if (effects) {
			eventData["effect"] = effects[m_EffectIndex];
		}

		return eventData;
	}
}
