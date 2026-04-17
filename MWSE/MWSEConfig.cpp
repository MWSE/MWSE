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
	bool Configuration::EnableDX8BatchRendering = true;
	bool Configuration::EnableDX8BatchGrouping = true;
	bool Configuration::EnableDX8BatchStateReset = true;
	bool Configuration::EnableDX8BatchWorldObjectRoot = true;
	bool Configuration::EnableDX8BatchWorldLandscapeRoot = true;
	bool Configuration::EnableDX8BatchWorldPickObjectRoot = true;
	bool Configuration::EnableDX8BatchWorldVFXRoot = true;
	bool Configuration::EnableDX8BatchWorldSpellRoot = true;
	bool Configuration::EnableDX8BatchWorldArmRoot = true;
	bool Configuration::EnableDX8BatchWorldProjectileRoot = true;
	bool Configuration::EnableDX8BatchWorldUnclassified = true;
#ifdef APPVEYOR_BUILD_NUMBER
	UINT Configuration::BuildNumber = APPVEYOR_BUILD_NUMBER;
#else
	constexpr auto DEV_BUILD_NUMBER = std::numeric_limits<unsigned short>::max();
	UINT Configuration::BuildNumber = DEV_BUILD_NUMBER;
	static_assert(DEV_BUILD_NUMBER == int(float(DEV_BUILD_NUMBER)), "Dev build number could not survive round-trip through mwscript.");
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
		DECLARE_CONFIG(EnableDX8BatchRendering)
		DECLARE_CONFIG(EnableDX8BatchGrouping)
		DECLARE_CONFIG(EnableDX8BatchStateReset)
		DECLARE_CONFIG(EnableDX8BatchWorldObjectRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldLandscapeRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldPickObjectRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldVFXRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldSpellRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldArmRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldProjectileRoot)
		DECLARE_CONFIG(EnableDX8BatchWorldUnclassified)
		DECLARE_CONFIG(BuildNumber)
	}
}
