#include "LuaRenderEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	RenderEvent::RenderEvent() :
		GenericEvent("render")
	{
	}

	sol::table RenderEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		return eventData;
	}
}
