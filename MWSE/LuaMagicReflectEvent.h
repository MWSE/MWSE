#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class MagicReflectEvent : public GenericEvent, public DisableableEvent<MagicReflectEvent> {
	public:
		MagicReflectEvent(TES3::MagicSourceInstance* sourceInstance, TES3::Reference* target, TES3::ActiveMagicEffect* reflectEffect, float reflectChance);
		sol::table createEventTable();

	protected:
		TES3::MagicSourceInstance* m_MagicSourceInstance;
		TES3::Reference* m_Target;
		TES3::ActiveMagicEffect* m_ReflectEffect;
		float m_ReflectChance;
	};
}
