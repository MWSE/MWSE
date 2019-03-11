#include "LuaEquipEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			EquipEvent::EquipEvent(TES3::Reference* reference, TES3::BaseObject* item, TES3::ItemData* itemData) :
				ObjectFilteredEvent("equip", item),
				m_Reference(reference),
				m_Item(item),
				m_ItemData(itemData)
			{

			}

			sol::table EquipEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["reference"] = makeLuaObject(m_Reference);
				eventData["item"] = makeLuaObject(m_Item);
				eventData["itemData"] = m_ItemData;

				return eventData;
			}
		}
	}
}
