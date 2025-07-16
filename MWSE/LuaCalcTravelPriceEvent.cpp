#include "LuaCalcTravelPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Attachment.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalculateTravelPriceEvent::CalculateTravelPriceEvent(TES3::MobileActor* mobileActor, int basePrice, int price, TES3::TravelDestination* destination, std::vector<TES3::MobileActor*>* companionList, float distance) :
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
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		if (m_MobileActor) {
			eventData["reference"] = m_MobileActor->reference;
		}

		//eventData["distance"] = m_Distance;
		eventData["destination"] = m_Destination->destination;

		if (!m_CompanionList->empty()) {
			sol::table companionList = state.create_table();
			for (size_t i = 0; i < m_CompanionList->size(); ++i) {
				auto companion = m_CompanionList->at(i);
				companionList[i + 1] = companion;
			}
			eventData["companions"] = companionList;
		}

		eventData["basePrice"] = m_BasePrice;
		eventData["price"] = m_Price;

		return eventData;
	}
}
