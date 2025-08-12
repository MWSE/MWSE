#include "LuaLevelUpEvent.h"

#include "LuaManager.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	LevelUpEvent::LevelUpEvent() :
		GenericEvent("levelUp")
	{

	}

	sol::table LevelUpEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		TES3::MobilePlayer* player = TES3::WorldController::get()->getMobilePlayer();

		eventData["level"] = player->reference->baseObject->getLevel();

		return eventData;
	}

	sol::object LevelUpEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		TES3::MobilePlayer* player = TES3::WorldController::get()->getMobilePlayer();

		options["filter"] = player->reference->baseObject->getLevel();

		return options;
	}
}
