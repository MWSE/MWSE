#include "MWSEUtilLua.h"

#include "LuaManager.h"

#include "BuildDate.h"
#include "MWSEDefs.h"
#include "WindowsUtil.h"

#include "LuaTimer.h"

namespace mwse::lua {
#ifdef APPVEYOR_BUILD_NUMBER
	constexpr unsigned int buildNumber = APPVEYOR_BUILD_NUMBER;
#else
	constexpr unsigned int buildNumber = UINT_MAX;
#endif

	void crash() {
		// You're not my manager!
		int* x = nullptr;
		*x = 4;
	}

	unsigned int getVersion() {
		return MWSE_VERSION_INTEGER;
	}

	bool overrideScript(const char* scriptId, sol::object target) {
		return LuaManager::getInstance().overrideScript(scriptId, target);
	}

	sol::object breakpoint() {
		while (ShowCursor(TRUE) < 0);
		return sol::nil;
	}

	void bindMWSEUtil() {
		auto& manager = LuaManager::getInstance();
		auto stateHandle = manager.getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;

		// Create our namespace.
		sol::table lua_mwse = state["mwse"];

		// Provide access to our timer collections.
		lua_mwse["realTimers"] = manager.realTimers;
		lua_mwse["simulateTimers"] = manager.simulateTimers;
		lua_mwse["gameTimers"] = manager.gameTimers;

		// Basic value binding.
		lua_mwse["buildDate"] = MWSE_BUILD_DATE;
		lua_mwse["buildNumber"] = buildNumber;
		lua_mwse["version"] = MWSE_VERSION_INTEGER;

		// Basic function binding.
		lua_mwse["breakpoint"] = breakpoint;
		lua_mwse["crash"] = crash;
		lua_mwse["getVersion"] = getVersion;
		lua_mwse["getVirtualMemoryUsage"] = getVirtualMemoryUsage;
		lua_mwse["overrideScript"] = overrideScript;
		lua_mwse["virtualKeyPressed"] = getIsVirtualKeyPressed;
	}
}
