#include "LuaCalcHitChanceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalcHitChanceEvent::CalcHitChanceEvent(TES3::MobileActor * attacker, int hitChance) :
				ObjectFilteredEvent("calcHitChance", attacker->reference->baseObject),
				m_Attacker(attacker),
				m_HitChance(hitChance)
			{

			}

			sol::table CalcHitChanceEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["attackerMobile"] = makeLuaObject(m_Attacker);
				eventData["attacker"] = makeLuaObject(m_Attacker->reference);

				auto target = m_Attacker->actionData.hitTarget;
				if (target) {
					eventData["targetMobile"] = makeLuaObject(target);
					eventData["target"] = makeLuaObject(target->reference);
				}

				eventData["hitChance"] = m_HitChance;

				return eventData;
			}
		}
	}
}
