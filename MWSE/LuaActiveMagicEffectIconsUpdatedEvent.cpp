#include "LuaActiveMagicEffectIconsUpdatedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	ActiveMagicEffectIconsUpdatedEvent::ActiveMagicEffectIconsUpdatedEvent() :
		GenericEvent("activeMagicEffectIconsUpdated")
	{

	}

	sol::table ActiveMagicEffectIconsUpdatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		return eventData;
	}
}
