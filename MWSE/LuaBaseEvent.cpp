
#include "LuaBaseEvent.h"

#include "LuaManager.h"

#include "Log.h"

namespace mwse::lua::event {
	sol::object trigger(const char* eventType, sol::table eventData, sol::object eventOptions) {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Trigger the function, check for lua errors.
		static sol::protected_function luaEventTrigger = state["event"]["trigger"];
		sol::protected_function_result result = luaEventTrigger(eventType, eventData, eventOptions);
		if (result.valid()) {
			return result;
		}
		else {
			sol::error error = result;
			log::getLog() << "Event system error encountered when raising " << eventType << " event:" << std::endl << error.what() << std::endl;
		}

		return sol::nil;
	}

	void clearObjectFilter(sol::object filterObject) {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		static sol::protected_function luaEventClear = state["event"]["clear"];
		luaEventClear(sol::nil, filterObject);
	}
}
