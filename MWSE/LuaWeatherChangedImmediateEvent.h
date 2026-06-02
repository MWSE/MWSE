#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class WeatherChangedImmediateEvent : public GenericEvent, public DisableableEvent<WeatherChangedImmediateEvent> {
	public:
		WeatherChangedImmediateEvent();
		sol::table createEventTable();

		static std::atomic<bool> ms_EventGuard;
	};
}
