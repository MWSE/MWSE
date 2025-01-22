#include "LuaCalcRepairPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalculateRepairPriceEvent::CalculateRepairPriceEvent(TES3::MobileActor* mobileActor, int basePrice, int price, TES3::Object* item, TES3::ItemData* itemData) :
		ObjectFilteredEvent("calcRepairPrice", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_BasePrice(basePrice),
		m_Price(price),
		m_Item(item),
		m_ItemData(itemData)
	{

	}

	sol::table CalculateRepairPriceEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		if (m_MobileActor) {
			eventData["reference"] = m_MobileActor->reference;
		}

		eventData["item"] = m_Item;
		eventData["itemData"] = m_ItemData;

		eventData["basePrice"] = m_BasePrice;
		eventData["price"] = m_Price;

		return eventData;
	}
}
