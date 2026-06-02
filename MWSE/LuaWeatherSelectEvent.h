#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace TES3 {
	struct Region;
}

namespace mwse::lua::event {
	class WeatherSelectEvent : public ObjectFilteredEvent, public DisableableEvent<WeatherSelectEvent> {
	public:
		WeatherSelectEvent(TES3::Region* region);
		sol::table createEventTable();

	protected:
		TES3::Region* m_Region;
	};
}
