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
		static bool EnableMSOC;
		static bool DebugOcclusionTintOccluded;
		static bool DebugOcclusionTintTested;
		static bool DebugOcclusionTintOccluder;
		static float OcclusionOccluderRadiusMin;
		static float OcclusionOccluderRadiusMax;
		static float OcclusionOccluderMinDimension;
		static float OcclusionInsideOccluderMargin;
		static float OcclusionDepthSlackWorldUnits;
		static UINT OcclusionOccluderMaxTriangles;
		static float OcclusionOccludeeMinRadius;
		static bool OcclusionEnableInterior;
		static bool OcclusionEnableExterior;
		static bool OcclusionSkipTerrainOccludees;
		static bool OcclusionAggregateTerrain;
		static UINT OcclusionTerrainResolution;
		static bool OcclusionCullLights;
		static UINT OcclusionLightCullHysteresisFrames;
		static bool OcclusionAsyncOccluders;
		static UINT OcclusionThreadpoolThreadCount;
		static UINT OcclusionThreadpoolBinsW;
		static UINT OcclusionThreadpoolBinsH;
		static UINT OcclusionTemporalCoherenceFrames;
		static bool OcclusionParallelDrain;
		static bool OcclusionLogPerFrame;
		static bool OcclusionLogAggregate;
		static UINT BuildNumber;

		static sol::table getDefaults();

		static void bindToLua();
	};
}
