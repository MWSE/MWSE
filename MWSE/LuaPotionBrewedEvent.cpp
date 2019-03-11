#include "LuaPotionBrewedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Alchemy.h"

namespace mwse {
	namespace lua {
		namespace event {
			PotionBrewedEvent::PotionBrewedEvent(TES3::Alchemy * object) :
				GenericEvent("potionBrewed"),
				m_Object(object)
			{

			}

			sol::table PotionBrewedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["object"] = makeLuaObject(m_Object);

				return eventData;
			}
		}
	}
}
