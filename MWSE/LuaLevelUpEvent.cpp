#include "LuaLevelUpEvent.h"

#include "LuaManager.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			LevelUpEvent::LevelUpEvent() :
				GenericEvent("levelUp")
			{

			}

			sol::table LevelUpEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				TES3::MobilePlayer* player = TES3::WorldController::get()->getMobilePlayer();

				eventData["level"] = player->reference->baseObject->getLevel();

				return eventData;
			}

			sol::object LevelUpEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				TES3::MobilePlayer* player = TES3::WorldController::get()->getMobilePlayer();

				options["filter"] = player->reference->baseObject->getLevel();

				return options;
			}
		}
	}
}
