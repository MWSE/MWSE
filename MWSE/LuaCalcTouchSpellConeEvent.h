#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class CalcTouchSpellConeEvent : public ObjectFilteredEvent, public DisableableEvent<CalcTouchSpellConeEvent> {
	public:
		CalcTouchSpellConeEvent(TES3::MobileActor* caster, TES3::MagicSourceInstance* sourceInstance, float distance, double angleXY, double angleZ);
		sol::table createEventTable();

	protected:
		TES3::MobileActor* m_Caster;
		TES3::MagicSourceInstance* m_MagicSourceInstance;
		float m_Reach;
		double m_AngleXY;
		double m_AngleZ;
	};
}
