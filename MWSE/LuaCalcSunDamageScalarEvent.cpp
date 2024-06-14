#include "LuaCalcSunDamageScalarEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3WeatherController.h"

namespace mwse::lua::event {
	CalcSunDamageScalarEvent::CalcSunDamageScalarEvent(float damage) :
		GenericEvent("calcSunDamageScalar"),
		m_Damage(damage)
	{

	}

	sol::table CalcSunDamageScalarEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["damage"] = m_Damage;

		return eventData;
	}
}
