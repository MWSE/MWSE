#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class RenderEvent : public GenericEvent, public DisableableEvent<RenderEvent> {
	public:
		RenderEvent();
		sol::table createEventTable();
	};
}
