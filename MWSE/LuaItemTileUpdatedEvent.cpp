#include "LuaItemTileUpdatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"

namespace mwse::lua::event {
	ItemTileUpdatedEvent::ItemTileUpdatedEvent(TES3::UI::InventoryTile* tile) :
		GenericEvent("itemTileUpdated"),
		m_Tile(tile)
	{

	}

	sol::table ItemTileUpdatedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["element"] = m_Tile->element;
		eventData["item"] = m_Tile->item;
		eventData["itemData"] = m_Tile->itemData;
		eventData["menu"] = m_Tile->element->getTopLevelParent();
		eventData["tile"] = m_Tile;

		return eventData;
	}

	sol::object ItemTileUpdatedEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();
		options["filter"] = m_Tile->element->getTopLevelParent()->name.cString;
		return options;
	}
}
