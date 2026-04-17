#pragma once

namespace mwse {
	class Configuration {
	public:
		static bool LogWarningsWithLuaStack;
		static bool EnableLegacyLuaMods;
		static bool KeepAllNetImmerseObjectsAlive;
		static bool RunInBackground;
		static bool PatchNiFlipController;
		static bool LetterboxMovies;
		static bool EnableLogColors;
		static bool EnableLogLineNumbers;
		static bool EnableDependencyChecks;
		static bool ReplaceDialogueFiltering;
		static bool EnableLuaErrorNotifications;
		static bool UseSkinnedAccurateActivationRaytests;
		static bool SuppressUselessWarnings;
		static bool UseGlobalAudio;
		static bool ReplaceLightSorting;
		static bool EnableDX8BatchRendering;
		static bool EnableDX8BatchGrouping;
		static bool EnableDX8BatchStateReset;
		static bool EnableDX8BatchWorldObjectRoot;
		static bool EnableDX8BatchWorldLandscapeRoot;
		static bool EnableDX8BatchWorldPickObjectRoot;
		static bool EnableDX8BatchWorldVFXRoot;
		static bool EnableDX8BatchWorldSpellRoot;
		static bool EnableDX8BatchWorldArmRoot;
		static bool EnableDX8BatchWorldProjectileRoot;
		static bool EnableDX8BatchWorldUnclassified;
		static UINT BuildNumber;

		static sol::table getDefaults();

		static void bindToLua();
	};
}
