#include "LuaPickpocketEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	PickpocketEvent::PickpocketEvent(TES3::MobileActor* mobile, TES3::Item* item, TES3::ItemData* itemData, int count, int chance) :
		GenericEvent("pickpocket"),
		m_MobileActor(mobile),
		m_Item(item),
		m_ItemData(itemData),
		m_Count(count),
		m_Chance(chance)
	{
	}

	sol::table PickpocketEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		eventData["reference"] = m_MobileActor->reference;
		eventData["item"] = m_Item;
		eventData["itemData"] = m_ItemData;
		eventData["count"] = m_Count;
		eventData["chance"] = m_Chance;

		return eventData;
	}
}
