#include "LuaFilterInventorySelectEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"

namespace mwse::lua::event {
	FilterInventorySelectEvent::FilterInventorySelectEvent(const char* type, TES3::Item* item, TES3::ItemData* itemData) :
		GenericEvent("filterInventorySelect"),
		m_Type(type),
		m_Item(item),
		m_ItemData(itemData)
	{

	}

	sol::table FilterInventorySelectEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["item"] = m_Item;
		eventData["itemData"] = m_ItemData;
		eventData["type"] = m_Type;

		return eventData;
	}

	sol::object FilterInventorySelectEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();
		options["filter"] = m_Type;
		return options;
	}
}
