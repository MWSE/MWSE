#include "LuaCalcHitChanceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalcHitChanceEvent::CalcHitChanceEvent(int hitChance) :
		ObjectFilteredEvent("calcHitChance", m_Attacker->reference->baseObject),
		m_HitChance(hitChance)
	{

	}

	sol::table CalcHitChanceEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["attackerMobile"] = m_Attacker;
		eventData["attacker"] = m_Attacker->reference;

		auto target = m_Attacker->actionData.hitTarget;
		if (target) {
			eventData["targetMobile"] = target;
			eventData["target"] = target->reference;
		}

		if (m_Projectile) {
			eventData["projectile"] = m_Projectile;
		}

		eventData["hitChance"] = m_HitChance;

		return eventData;
	}
	TES3::MobileActor* CalcHitChanceEvent::m_Attacker = nullptr;
	TES3::MobileProjectile* CalcHitChanceEvent::m_Projectile = nullptr;
}
