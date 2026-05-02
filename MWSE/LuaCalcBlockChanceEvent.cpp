#include "LuaCalcBlockChanceEvent.h"

#include "LuaManager.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalcBlockChanceEvent::CalcBlockChanceEvent(TES3::MobileActor* attacker, int blockChance) :
		ObjectFilteredEvent("calcBlockChance", attacker->reference->baseObject),
		m_Attacker(attacker),
		m_BlockChance(blockChance)
	{

	}

	sol::table CalcBlockChanceEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["attackerMobile"] = m_Attacker;
		eventData["attacker"] = m_Attacker->reference;

		auto target = m_Attacker->actionData.hitTarget;
		if (target) {
			eventData["targetMobile"] = target;
			eventData["target"] = target->reference;
		}

		eventData["blockChance"] = m_BlockChance;

		return eventData;
	}
}
