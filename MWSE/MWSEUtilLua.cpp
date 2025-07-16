#include "MWSEUtilLua.h"

#include "LuaManager.h"

#include "BuildDate.h"
#include "MWSEDefs.h"
#include "UTF8Convert.h"
#include "WindowsUtil.h"

#include "LuaTimer.h"

#include "Log.h"

#include "TES3InputController.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3WorldController.h"

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

	bool clearScriptOverride(const char* scriptId) {
		return LuaManager::getInstance().clearScriptOverride(scriptId);
	}

	void forceCursorOn() {
		TES3::WorldController::get()->inputController->mouse->SetCooperativeLevel(GetActiveWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		TES3::WorldController::get()->inputController->keyboard->SetCooperativeLevel(GetActiveWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
		while (ShowCursor(TRUE) < 0);
	}

	sol::object breakpoint(sol::optional<const char*> message) {
		if (message.value_or(nullptr)) {
			mwse::log::getLog() << "[MWSE] Hit breakpoint: " << message.value() << std::endl;
		}
		return sol::nil;
	}

	std::tuple<TES3::Script*,TES3::Reference*> getCurrentMorrowindScriptState() {
		return { TES3::Script::currentlyExecutingScript, TES3::Script::currentlyExecutingScriptReference };
	}

	void bindMWSEUtil() {
		auto& manager = LuaManager::getInstance();
		const auto stateHandle = manager.getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

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
#if _DEBUG
		lua_mwse["debugBuild"] = true;
#else
		lua_mwse["debugBuild"] = false;
#endif

		// Basic function binding.
		lua_mwse["breakpoint"] = breakpoint;
		lua_mwse["clearScriptOverride"] = clearScriptOverride;
		lua_mwse["crash"] = crash;
		lua_mwse["forceCursorOn"] = forceCursorOn;
		lua_mwse["getCurrentMorrowindScriptState"] = getCurrentMorrowindScriptState;
		lua_mwse["getVersion"] = getVersion;
		lua_mwse["getVirtualMemoryUsage"] = getVirtualMemoryUsage;
		lua_mwse["iconv"] = iconv;
		lua_mwse["overrideScript"] = overrideScript;
		lua_mwse["virtualKeyPressed"] = getIsVirtualKeyPressed;
	}
}
