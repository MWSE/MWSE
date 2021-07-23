#include "LuaPreLevelUpEvent.h"

#include "LuaManager.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			PreLevelUpEvent::PreLevelUpEvent() :
				GenericEvent("preLevelUp")
			{

			}

			sol::table PreLevelUpEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				TES3::MobilePlayer* player = TES3::WorldController::get()->getMobilePlayer();

				eventData["level"] = player->reference->baseObject->getLevel() + 1;

				return eventData;
			}

			sol::object PreLevelUpEvent::getEventOptions() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table options = state.create_table();

				TES3::MobilePlayer* player = TES3::WorldController::get()->getMobilePlayer();

				options["filter"] = player->reference->baseObject->getLevel() + 1;

				return options;
			}

			bool PreLevelUpEvent::m_EventEnabled = false;
		}
	}
}
