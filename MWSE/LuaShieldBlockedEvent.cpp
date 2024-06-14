#include "LuaShieldBlockedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	ShieldBlockedEvent::ShieldBlockedEvent(TES3::MobileActor* mobileActor, TES3::MobileActor* attacker, float damage) :
		ObjectFilteredEvent("shieldBlocked", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_Attacker(attacker),
		m_ConditionDamage(damage)
	{

	}

	sol::table ShieldBlockedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["conditionDamage"] = m_ConditionDamage;
		eventData["attacker"] = m_Attacker;
		eventData["mobile"] = m_MobileActor;
		eventData["reference"] = m_MobileActor->reference;

		return eventData;
	}
}
