#include "LuaFilterBarterMenuEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3UIInventoryTile.h"

namespace mwse::lua::event {
	FilterBarterMenuEvent::FilterBarterMenuEvent(TES3::UI::InventoryTile* tile, TES3::Item* item) :
		ObjectFilteredEvent("filterBarterMenu", item),
		m_Tile(tile),
		m_Item(item)
	{

	}

	sol::table FilterBarterMenuEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["tile"] = m_Tile;
		eventData["item"] = m_Item;
		eventData["itemData"] = m_Tile->itemData;

		return eventData;
	}
}
