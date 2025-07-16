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
	bool Configuration::LetterboxMovies = false;
	bool Configuration::EnableLogColors = false;
	bool Configuration::EnableLogLineNumbers = false;
	bool Configuration::EnableDependencyChecks = true;
	bool Configuration::ReplaceDialogueFiltering = true;
	bool Configuration::EnableLuaErrorNotifications = false;
	bool Configuration::UseSkinnedAccurateActivationRaytests = true;
	bool Configuration::SuppressUselessWarnings = true;
	bool Configuration::UseGlobalAudio = false;
	bool Configuration::ReplaceLightSorting = true;
#ifdef APPVEYOR_BUILD_NUMBER
	UINT Configuration::BuildNumber = APPVEYOR_BUILD_NUMBER;
#else
	UINT Configuration::BuildNumber = UINT_MAX;
#endif


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
		const auto stateHandle = lua::LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

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
		DECLARE_CONFIG(LetterboxMovies)
		DECLARE_CONFIG(EnableLogColors)
		DECLARE_CONFIG(EnableLogLineNumbers)
		DECLARE_CONFIG(EnableDependencyChecks)
		DECLARE_CONFIG(ReplaceDialogueFiltering)
		DECLARE_CONFIG(EnableLuaErrorNotifications)
		DECLARE_CONFIG(UseSkinnedAccurateActivationRaytests)
		DECLARE_CONFIG(SuppressUselessWarnings)
		DECLARE_CONFIG(UseGlobalAudio)
		DECLARE_CONFIG(ReplaceLightSorting)
		DECLARE_CONFIG(BuildNumber)
	}
}
