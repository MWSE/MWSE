#include "LuaMagicAbsorbEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"
#include "TES3MobileActor.h"

namespace mwse::lua::event {
	MagicAbsorbEvent::MagicAbsorbEvent(TES3::MagicSourceInstance* sourceInstance, TES3::Reference* target, TES3::ActiveMagicEffect* absorbEffect, float absorbChance) :
		GenericEvent("magicAbsorb"),
		m_MagicSourceInstance(sourceInstance),
		m_Target(target),
		m_AbsorbEffect(absorbEffect),
		m_AbsorbChance(absorbChance)
	{
	}

	sol::table MagicAbsorbEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		auto mobile = m_Target->getAttachedMobileActor();

		eventData["target"] = m_Target;
		eventData["mobile"] = mobile;
		eventData["absorbEffect"] = TES3::ActiveMagicEffectLua(*m_AbsorbEffect, mobile);
		eventData["absorbChance"] = m_AbsorbChance;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;

		return eventData;
	}
}
