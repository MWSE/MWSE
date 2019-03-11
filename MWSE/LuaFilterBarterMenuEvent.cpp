#include "LuaFilterBarterMenuEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3UIInventoryTile.h"

namespace mwse {
	namespace lua {
		namespace event {
			FilterBarterMenuEvent::FilterBarterMenuEvent(TES3::UI::InventoryTile * tile, TES3::Item * item) :
				ObjectFilteredEvent("filterBarterMenu", item),
				m_Tile(tile),
				m_Item(item)
			{

			}

			sol::table FilterBarterMenuEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["tile"] = m_Tile;
				eventData["item"] = makeLuaObject(m_Item);
				eventData["itemData"] = m_Tile->itemData;

				return eventData;
			}
		}
	}
}
