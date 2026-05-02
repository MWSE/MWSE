#include "LuaWeatherChangedImmediateEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	WeatherChangedImmediateEvent::WeatherChangedImmediateEvent() :
		GenericEvent("weatherChangedImmediate")
	{

	}

	sol::table WeatherChangedImmediateEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		auto controller = TES3::WorldController::get()->weatherController;

		eventData["to"] = controller->currentWeather;

		return eventData;
	}
}
