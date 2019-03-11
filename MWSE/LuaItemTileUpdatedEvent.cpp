#include "LuaItemTileUpdatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"

namespace mwse {
	namespace lua {
		namespace event {
			ItemTileUpdatedEvent::ItemTileUpdatedEvent(TES3::UI::InventoryTile * tile) :
				GenericEvent("itemTileUpdated"),
				m_Tile(tile)
			{

			}

			sol::table ItemTileUpdatedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["element"] = m_Tile->element;
				eventData["item"] = makeLuaObject(m_Tile->item);
				eventData["itemData"] = m_Tile->itemData;
				eventData["menu"] = m_Tile->element->getTopLevelParent();
				eventData["tile"] = m_Tile;

				return eventData;
			}

			sol::object ItemTileUpdatedEvent::getEventOptions() {
				sol::table options = LuaManager::getInstance().createTable();
				options["filter"] = m_Tile->element->getTopLevelParent()->name.cString;
				return options;
			}
		}
	}
}
