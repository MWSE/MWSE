#include "LuaWeatherChangedImmediateEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Weather.h"
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
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				auto controller = TES3::WorldController::get()->weatherController;

				eventData["to"] = controller->currentWeather;

				return eventData;
			}

			bool WeatherChangedImmediateEvent::m_EventEnabled = false;
		}
	}
}
