#include "LuaMagicReflectEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"
#include "TES3MobileActor.h"

namespace mwse::lua::event {
	MagicReflectEvent::MagicReflectEvent(TES3::MagicSourceInstance* sourceInstance, TES3::Reference* target, TES3::ActiveMagicEffect* reflectEffect, float reflectChance) :
		GenericEvent("magicReflect"),
		m_MagicSourceInstance(sourceInstance),
		m_Target(target),
		m_ReflectEffect(reflectEffect),
		m_ReflectChance(reflectChance)
	{
	}

	sol::table MagicReflectEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		auto mobile = m_Target->getAttachedMobileActor();

		eventData["target"] = m_Target;
		eventData["mobile"] = mobile;
		eventData["reflectEffect"] = TES3::ActiveMagicEffectLua(*m_ReflectEffect, mobile);
		eventData["reflectChance"] = m_ReflectChance;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;

		return eventData;
	}
}
