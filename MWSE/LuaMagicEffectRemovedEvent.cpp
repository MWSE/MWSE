#include "LuaMagicEffectRemovedEvent.h"

#include "LuaManager.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MagicEffectInstance.h"

namespace mwse::lua::event {
	MagicEffectRemovedEvent::MagicEffectRemovedEvent(TES3::MobileActor* mobileActor, TES3::MagicSourceInstance* magicSourceInstance, int effectIndex) :
		ObjectFilteredEvent("magicEffectRemoved", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_MagicSourceInstance(magicSourceInstance),
		m_EffectIndex(effectIndex)
	{

	}

	sol::table MagicEffectRemovedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		eventData["reference"] = m_MobileActor->reference;
		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_MagicSourceInstance->target;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["effectIndex"] = m_EffectIndex;
		eventData["effectInstance"] = m_MagicSourceInstance->getEffectInstance(m_EffectIndex, m_MobileActor->reference);

		// Get the specific effect on the source.
		TES3::Effect* effects = m_MagicSourceInstance->sourceCombo.getSourceEffects();
		if (effects) {
			eventData["effect"] = effects[m_EffectIndex];
		}

		return eventData;
	}
}
