#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class MagicAbsorbEvent : public GenericEvent, public DisableableEvent<MagicAbsorbEvent> {
	public:
		MagicAbsorbEvent(TES3::MagicSourceInstance* sourceInstance, TES3::Reference* target, TES3::ActiveMagicEffect* absorbEffect, float absorbChance);
		sol::table createEventTable();

	protected:
		TES3::MagicSourceInstance* m_MagicSourceInstance;
		TES3::Reference* m_Target;
		TES3::ActiveMagicEffect* m_AbsorbEffect;
		float m_AbsorbChance;
	};
}
