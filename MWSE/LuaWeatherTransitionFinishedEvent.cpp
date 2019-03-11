#include "LuaWeatherTransitionFinishedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			WeatherTransitionFinishedEvent::WeatherTransitionFinishedEvent() :
				GenericEvent("weatherTransitionFinished")
			{

			}

			sol::table WeatherTransitionFinishedEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();
				auto controller = TES3::WorldController::get()->weatherController;

				eventData["to"] = makeLuaObject(controller->currentWeather);

				return eventData;
			}
		}
	}
}
