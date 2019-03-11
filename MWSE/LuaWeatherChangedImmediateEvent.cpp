#include "LuaWeatherChangedImmediateEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			WeatherChangedImmediateEvent::WeatherChangedImmediateEvent() :
				GenericEvent("weatherChangedImmediate")
			{

			}

			sol::table WeatherChangedImmediateEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();
				auto controller = TES3::WorldController::get()->weatherController;

				eventData["to"] = makeLuaObject(controller->currentWeather);

				return eventData;
			}
		}
	}
}
