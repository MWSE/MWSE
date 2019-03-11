#include "LuaCalcTrainingPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateTrainingPriceEvent::CalculateTrainingPriceEvent(TES3::MobileActor * mobileActor, int basePrice, int price, int skillId) :
				ObjectFilteredEvent("calcTrainingPrice", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_BasePrice(basePrice),
				m_Price(price),
				m_SkillId(skillId)
			{

			}

			sol::table CalculateTrainingPriceEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				if (m_MobileActor) {
					eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				}

				eventData["skill"] = TES3::WorldController::get()->getMobilePlayer()->getSkillStatistic(m_SkillId);
				eventData["skillId"] = m_SkillId;

				eventData["basePrice"] = m_BasePrice;
				eventData["price"] = m_Price;

				return eventData;
			}
		}
	}
}
