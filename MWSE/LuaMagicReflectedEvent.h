#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class MagicReflectedEvent : public GenericEvent, public DisableableEvent<MagicReflectedEvent> {
	public:
		MagicReflectedEvent(TES3::MagicSourceInstance* sourceInstance, TES3::Reference* target, TES3::ActiveMagicEffect* reflectEffect);
		sol::table createEventTable();

	protected:
		TES3::MagicSourceInstance* m_MagicSourceInstance;
		TES3::Reference* m_Target;
		TES3::ActiveMagicEffect* m_ReflectEffect;
	};
}
