#include "LuaFilterInventorySelectEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Item.h"
#include "TES3ItemData.h"

namespace mwse {
	namespace lua {
		namespace event {
			FilterInventorySelectEvent::FilterInventorySelectEvent(const char * type, TES3::Item * item, TES3::ItemData * itemData) :
				GenericEvent("filterInventorySelect"),
				m_Type(type),
				m_Item(item),
				m_ItemData(itemData)
			{

			}

			sol::table FilterInventorySelectEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["item"] = makeLuaObject(m_Item);
				eventData["itemData"] = m_ItemData;
				eventData["type"] = m_Type;

				return eventData;
			}

			sol::object FilterInventorySelectEvent::getEventOptions() {
				sol::table options = LuaManager::getInstance().createTable();
				options["filter"] = m_Type;
				return options;
			}
		}
	}
}
