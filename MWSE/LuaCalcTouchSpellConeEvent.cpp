#include "LuaCalcTouchSpellConeEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicSourceInstance.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalcTouchSpellConeEvent::CalcTouchSpellConeEvent(TES3::MobileActor* caster, TES3::MagicSourceInstance* sourceInstance, float reach, double angleXY, double angleZ) :
		ObjectFilteredEvent("calcTouchSpellCone", caster->reference),
		m_Caster(caster),
		m_MagicSourceInstance(sourceInstance),
		m_Reach(reach),
		m_AngleXY(angleXY),
		m_AngleZ(angleZ)
	{

	}

	sol::table CalcTouchSpellConeEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["casterMobile"] = m_Caster;
		eventData["caster"] = m_Caster->reference;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["reach"] = m_Reach;
		eventData["angleXY"] = m_AngleXY;
		eventData["angleZ"] = m_AngleZ;

		return eventData;
	}
}
