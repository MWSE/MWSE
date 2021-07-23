#include "MWSEConfig.h"

#include "LuaManager.h"

#define DECLARE_CONFIG(cfg) bindConfig(usertypeDefinition, #cfg, Configuration::cfg);

namespace mwse {
	// Declare default values.
	bool Configuration::EnableLegacyLuaMods = true;
	bool Configuration::LogWarningsWithLuaStack = true;
	bool Configuration::KeepAllNetImmerseObjectsAlive = false;
	bool Configuration::RunInBackground = false;
	bool Configuration::PatchNiFlipController = true;

	// Allow default values to be accessed later.
	sol::table defaultConfig;
	sol::table Configuration::getDefaults() {
		return defaultConfig;
	}

	template <typename T>
	constexpr void bindConfig(sol::usertype<Configuration>& usertypeDefinition, const char* key, T& value) {
		usertypeDefinition[key] = sol::var(std::ref(value));
		defaultConfig[key] = value;
	}

	// Let lua muck with all this.
	void Configuration::bindToLua() {
		// Get our lua state.
		auto stateHandle = lua::LuaManager::getInstance().getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;

		defaultConfig = state.create_table();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<Configuration>("mwseConfig");
		usertypeDefinition["new"] = sol::no_constructor;
		usertypeDefinition["getDefaults"] = &Configuration::getDefaults;

		// Bind all of our config entries.
		DECLARE_CONFIG(EnableLegacyLuaMods)
		DECLARE_CONFIG(LogWarningsWithLuaStack)
		DECLARE_CONFIG(KeepAllNetImmerseObjectsAlive)
		DECLARE_CONFIG(RunInBackground)
		DECLARE_CONFIG(PatchNiFlipController)
	}
}
