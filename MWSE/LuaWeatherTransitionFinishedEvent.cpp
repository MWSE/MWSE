#include "LuaWeatherTransitionFinishedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	WeatherTransitionFinishedEvent::WeatherTransitionFinishedEvent() :
		GenericEvent("weatherTransitionFinished")
	{

	}

	sol::table WeatherTransitionFinishedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		auto controller = TES3::WorldController::get()->weatherController;

		eventData["to"] = controller->currentWeather;

		return eventData;
	}
}
