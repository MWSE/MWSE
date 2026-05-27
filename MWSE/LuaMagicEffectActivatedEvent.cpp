#include "LuaMagicEffectActivatedEvent.h"

#include "LuaManager.h"

#include "TES3MagicEffect.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MagicEffectActivatedEvent::MagicEffectActivatedEvent(TES3::MagicSourceInstance* magicSourceInstance, TES3::MagicEffectInstance* magicEffectInstance, int effectIndex) :
		GenericEvent("magicEffectActivated"),
		m_MagicSourceInstance(magicSourceInstance),
		m_MagicEffectInstance(magicEffectInstance),
		m_EffectIndex(effectIndex),
		m_EffectId(-1)
	{
		TES3::Effect* effects = m_MagicSourceInstance ? m_MagicSourceInstance->sourceCombo.getSourceEffects() : nullptr;
		if (effects) {
			m_EffectId = effects[m_EffectIndex].effectID;
		}
	}

	sol::table MagicEffectActivatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_MagicEffectInstance->target;
		eventData["effectId"] = m_EffectId;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["effectIndex"] = m_EffectIndex;
		eventData["effectInstance"] = m_MagicEffectInstance;
		eventData["state"] = m_MagicEffectInstance->state;

		TES3::Effect* effects = m_MagicSourceInstance->sourceCombo.getSourceEffects();
		if (effects) {
			eventData["effect"] = effects[m_EffectIndex];
		}

		return eventData;
	}

	sol::object MagicEffectActivatedEvent::getEventOptions() {
		if (m_EffectId < 0) {
			return sol::nil;
		}

		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();
		options["filter"] = m_EffectId;
		return options;
	}
}
