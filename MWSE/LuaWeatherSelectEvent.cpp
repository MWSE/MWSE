#include "LuaWeatherSelectEvent.h"

#include "LuaManager.h"

#include "TES3Region.h"

namespace mwse::lua::event {
	WeatherSelectEvent::WeatherSelectEvent(TES3::Region* region) :
		ObjectFilteredEvent("weatherSelect", region),
		m_Region(region)
	{
	}

	sol::table WeatherSelectEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();
		auto chances = state.create_table();

		for (int weatherIndex = 0; weatherIndex < 10; ++weatherIndex) {
			chances[weatherIndex] = m_Region->weatherChances[weatherIndex];
		}

		eventData["region"] = m_Region;
		eventData["chances"] = chances;
		return eventData;
	}
}
