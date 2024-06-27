#include "LuaSpellTickEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	SpellTickEvent::SpellTickEvent(int effectId, TES3::MagicSourceInstance* sourceInstance, float deltaTime, TES3::MagicEffectInstance* effectInstance, int effectIndex) :
		GenericEvent("spellTick"),
		m_EffectId(effectId),
		m_SourceInstance(sourceInstance),
		m_DeltaTime(deltaTime),
		m_EffectInstance(effectInstance),
		m_EffectIndex(effectIndex)
	{

	}

	sol::table SpellTickEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["caster"] = m_SourceInstance->caster;
		eventData["target"] = m_EffectInstance->target;

		eventData["effectId"] = m_EffectId;
		eventData["source"] = m_SourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_SourceInstance;
		eventData["deltaTime"] = m_DeltaTime;
		eventData["effectIndex"] = m_EffectIndex;
		eventData["effectInstance"] = m_EffectInstance;

		// Get the specific effect on the source.
		TES3::Effect* effects = m_SourceInstance->sourceCombo.getSourceEffects();
		if (effects) {
			eventData["effect"] = effects[m_EffectIndex];
		}

		return eventData;
	}

	sol::object SpellTickEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_SourceInstance->sourceCombo.source.asGeneric;

		return options;
	}
}
