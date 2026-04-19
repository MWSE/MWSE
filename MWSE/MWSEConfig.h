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
		static bool EnableDX8OcclusionCulling;
		static bool DebugOcclusionTintOccluded;
		static bool DebugOcclusionTintTested;
		static bool DebugOcclusionTintOccluder;
		static UINT BuildNumber;

		static sol::table getDefaults();

		static void bindToLua();
	};
}
