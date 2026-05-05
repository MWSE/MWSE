#include "LuaMagicEffectRemovedEvent.h"

#include "LuaManager.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MagicEffectRemovedEvent::MagicEffectRemovedEvent(TES3::MagicSourceInstance* magicSourceInstance, TES3::MagicEffectInstance* magicEffectInstance, int effectIndex) :
		ObjectFilteredEvent("magicEffectRemoved", magicSourceInstance ? magicSourceInstance->sourceCombo.source.asGeneric : nullptr),
		m_MagicSourceInstance(magicSourceInstance),
		m_MagicEffectInstance(magicEffectInstance),
		m_EffectIndex(effectIndex)
	{

	}

	sol::table MagicEffectRemovedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_MagicEffectInstance->target;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["effectIndex"] = m_EffectIndex;
		eventData["effectInstance"] = m_MagicEffectInstance;
		eventData["state"] = m_MagicEffectInstance->state;

		TES3::Effect* effects = m_MagicSourceInstance->sourceCombo.getSourceEffects();
		if (effects) {
			eventData["effect"] = effects[m_EffectIndex];
		}

		// Legacy bindings.
		eventData["reference"] = m_MagicEffectInstance->target;
		eventData["mobile"] = m_MagicEffectInstance->target ? m_MagicEffectInstance->target->getAttachedMobileActor() : nullptr;

		return eventData;
	}
}
