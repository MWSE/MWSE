#include "LuaEquipEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	EquipEvent::EquipEvent(TES3::Reference* reference, TES3::BaseObject* item, TES3::ItemData* itemData) :
		ObjectFilteredEvent("equip", item),
		m_Reference(reference),
		m_Item(item),
		m_ItemData(itemData)
	{

	}

	sol::table EquipEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;
		eventData["item"] = m_Item;
		eventData["itemData"] = m_ItemData;

		return eventData;
	}
}
