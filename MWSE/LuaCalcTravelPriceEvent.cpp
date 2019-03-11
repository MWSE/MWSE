#include "LuaCalcTravelPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Attachment.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateTravelPriceEvent::CalculateTravelPriceEvent(TES3::MobileActor * mobileActor, int basePrice, int price, TES3::TravelDestination * destination, std::vector<TES3::MobileActor*>* companionList, float distance) :
				ObjectFilteredEvent("calcTravelPrice", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_BasePrice(basePrice),
				m_Price(price),
				m_Destination(destination),
				m_CompanionList(companionList),
				m_Distance(distance)
			{

			}

			sol::table CalculateTravelPriceEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				if (m_MobileActor) {
					eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				}

				//eventData["distance"] = m_Distance;
				eventData["destination"] = makeLuaObject(m_Destination->destination);

				if (!m_CompanionList->empty()) {
					sol::table companionList = LuaManager::getInstance().createTable();
					for (size_t i = 0; i < m_CompanionList->size(); i++) {
						auto companion = m_CompanionList->at(i);
						companionList[i + 1] = makeLuaObject(companion);
					}
					eventData["companions"] = companionList;
				}

				eventData["basePrice"] = m_BasePrice;
				eventData["price"] = m_Price;

				return eventData;
			}
		}
	}
}
