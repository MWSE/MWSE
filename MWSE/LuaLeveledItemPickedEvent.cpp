#include "LuaLeveledItemPickedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3LeveledList.h"

namespace mwse {
	namespace lua {
		namespace event {
			LeveledItemPickedEvent::LeveledItemPickedEvent(TES3::LeveledItem * list, TES3::Object * vanillaResult) :
				ObjectFilteredEvent("leveledItemPicked", list),
				m_List(list),
				m_Result(vanillaResult)
			{

			}

			sol::table LeveledItemPickedEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["list"] = lua::makeLuaObject(m_List);
				eventData["pick"] = lua::makeLuaObject(m_Result);

				return eventData;
			}
		}
	}
}
