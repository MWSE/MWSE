local function saveConfig()
	local values = {}
	for k, _ in pairs(mwseConfig.getDefaults()) do
		values[k] = mwseConfig[k]
	end
	mwse.saveConfig("MWSE", values)
end

local i18n = mwse.loadTranslations("mwse.config")

-- Cached by the ThreadCount slider's postCreate hook. Lets the Bins
-- sliders' callbacks redraw the ThreadCount widget on clamp.
local threadCountSlider = nil

local function captureThreadCountSlider(self)
	threadCountSlider = self
end

local function clampOcclusionThreadCount()
	if mwseConfig.OcclusionThreadpoolThreadCount == 0 then return end
	local maxThreads = mwseConfig.OcclusionThreadpoolBinsW * mwseConfig.OcclusionThreadpoolBinsH
	if mwseConfig.OcclusionThreadpoolThreadCount > maxThreads then
		if threadCountSlider then
			-- setVariableValue writes the variable AND refreshes the
			-- widget + label, so the drag snaps to the clamped value.
			threadCountSlider:setVariableValue(maxThreads)
		else
			mwseConfig.OcclusionThreadpoolThreadCount = maxThreads
		end
	end
end

local function resetLighting()
	if not tes3.player then return end

	if (tes3.dataHandler.currentInteriorCell) then
		for reference in tes3.dataHandler.currentInteriorCell:iterateReferences() do
			reference:updateLighting()
		end
	else
		tes3.dataHandler:updateLightingForExteriorCells()
	end
end

local config = {
	name = "Morrowind Script Extender",
	template = "Template",
	pages = {
		{
			label = "Main",
			class = "SideBarPage",
			components = {
				{
					class = "OnOffButton",
					label = i18n("logWarningsWithLuaStack.label"),
					description = i18n("logWarningsWithLuaStack.description"),
					variable = {
						id = "LogWarningsWithLuaStack",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("runInBackground.label"),
					description = i18n("runInBackground.description"),
					variable = {
						id = "RunInBackground",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("playAudioInBackground.label"),
					description = i18n("playAudioInBackground.description"),
					variable = {
						id = "UseGlobalAudio",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("letterboxMovies.label"),
					description = i18n("letterboxMovies.description"),
					variable = {
						id = "LetterboxMovies",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("replaceDialogueFiltering.label"),
					description = i18n("replaceDialogueFiltering.description"),
					variable = {
						id = "ReplaceDialogueFiltering",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("patchNiFlipController.label"),
					description = i18n("patchNiFlipController.description"),
					variable = {
						id = "PatchNiFlipController",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("keepAllNetImmerseObjectsAlive.label"),
					description = i18n("keepAllNetImmerseObjectsAlive.description"),
					variable = {
						id = "KeepAllNetImmerseObjectsAlive",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("enableLegacyLuaMods.label"),
					description = i18n("enableLegacyLuaMods.description"),
					variable = {
						id = "EnableLegacyLuaMods",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("enableLogColors.label"),
					description = i18n("enableLogColors.description"),
					variable = {
						id = "EnableLogColors",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("enableLogLineNumbers.label"),
					description = i18n("enableLogLineNumbers.description"),
					variable = {
						id = "EnableLogLineNumbers",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("enableDependencyChecks.label"),
					description = i18n("enableDependencyChecks.description"),
					variable = {
						id = "EnableDependencyChecks",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("enableLuaErrorNotifications.label"),
					description = i18n("enableLuaErrorNotifications.description"),
					variable = {
						id = "EnableLuaErrorNotifications",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("useSkinnedAccurateActivationRaytests.label"),
					description = i18n("useSkinnedAccurateActivationRaytests.description"),
					variable = {
						id = "UseSkinnedAccurateActivationRaytests",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("suppressUselessWarnings.label"),
					description = i18n("suppressUselessWarnings.description"),
					variable = {
						id = "SuppressUselessWarnings",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					class = "OnOffButton",
					label = i18n("replaceLightSorting.label"),
					description = i18n("replaceLightSorting.description"),
					variable = {
						id = "ReplaceLightSorting",
						class = "TableVariable",
						table = mwseConfig,
					},
					callback = resetLighting,
				},
			},
			sidebarComponents = {
				{
					class = "Info",
					label = i18n("notice.label"),
					text = i18n("notice.text"),
				},
				{
					class = "Info",
					label = i18n("credits.label"),
					text = [[Anthony Garcia
Charles Cooley (cdcooley)
Cody Erekson (Fliggerty)
FreshFish
Grant McDorman
Greatness7
Hrnchamd
Merlord
Merzasphor
Michael Wallar (NullCascade)
OperatorJack
Pete Goodfellow (Petethegoat)
Pierre Steeg
Sebastien Levy (MetaBarj0)
Tim Peters
Timeslip]],
				},
			},
		},
		{
			label = i18n("msoc.pageLabel"),
			class = "SideBarPage",
			components = {
				{
					class = "Category",
					label = i18n("msoc.mainCategory.label"),
					components = {
						{
							class = "OnOffButton",
							label = i18n("enableMSOC.label"),
							description = i18n("enableMSOC.description"),
							variable = { id = "EnableMSOC", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("occlusionEnableInterior.label"),
							description = i18n("occlusionEnableInterior.description"),
							variable = { id = "OcclusionEnableInterior", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("occlusionEnableExterior.label"),
							description = i18n("occlusionEnableExterior.description"),
							variable = { id = "OcclusionEnableExterior", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("occlusionSkipTerrainOccludees.label"),
							description = i18n("occlusionSkipTerrainOccludees.description"),
							variable = { id = "OcclusionSkipTerrainOccludees", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("occlusionAggregateTerrain.label"),
							description = i18n("occlusionAggregateTerrain.description"),
							variable = { id = "OcclusionAggregateTerrain", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionTemporalCoherenceFrames.label"),
							description = i18n("occlusionTemporalCoherenceFrames.description"),
							min = 0, max = 10, step = 1, jump = 1,
							variable = { id = "OcclusionTemporalCoherenceFrames", class = "TableVariable", table = mwseConfig },
						},
					},
				},
				{
					class = "Category",
					label = i18n("msoc.occluderCategory.label"),
					components = {
						{
							class = "Slider",
							label = i18n("occlusionOccluderRadiusMin.label"),
							description = i18n("occlusionOccluderRadiusMin.description"),
							min = 0, max = 2048, step = 16, jump = 128,
							variable = { id = "OcclusionOccluderRadiusMin", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionOccluderRadiusMax.label"),
							description = i18n("occlusionOccluderRadiusMax.description"),
							min = 256, max = 16384, step = 128, jump = 1024,
							variable = { id = "OcclusionOccluderRadiusMax", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionOccluderMinDimension.label"),
							description = i18n("occlusionOccluderMinDimension.description"),
							min = 0, max = 1024, step = 8, jump = 64,
							variable = { id = "OcclusionOccluderMinDimension", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionOccluderMaxTriangles.label"),
							description = i18n("occlusionOccluderMaxTriangles.description"),
							min = 64, max = 16384, step = 64, jump = 512,
							variable = { id = "OcclusionOccluderMaxTriangles", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionInsideOccluderMargin.label"),
							description = i18n("occlusionInsideOccluderMargin.description"),
							min = 0, max = 512, step = 8, jump = 32,
							variable = { id = "OcclusionInsideOccluderMargin", class = "TableVariable", table = mwseConfig },
						},
					},
				},
				{
					class = "Category",
					label = i18n("msoc.occludeeCategory.label"),
					components = {
						{
							class = "Slider",
							label = i18n("occlusionDepthSlackWorldUnits.label"),
							description = i18n("occlusionDepthSlackWorldUnits.description"),
							min = 0, max = 1024, step = 8, jump = 32,
							variable = { id = "OcclusionDepthSlackWorldUnits", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionOccludeeMinRadius.label"),
							description = i18n("occlusionOccludeeMinRadius.description"),
							min = 0, max = 256, step = 1, jump = 16,
							variable = { id = "OcclusionOccludeeMinRadius", class = "TableVariable", table = mwseConfig },
						},
					},
				},
				{
					class = "Category",
					label = i18n("msoc.threadpoolCategory.label"),
					components = {
						{
							class = "OnOffButton",
							label = i18n("occlusionAsyncOccluders.label"),
							description = i18n("occlusionAsyncOccluders.description"),
							variable = { id = "OcclusionAsyncOccluders", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "Slider",
							label = i18n("occlusionThreadpoolThreadCount.label"),
							description = i18n("occlusionThreadpoolThreadCount.description"),
							min = 0, max = 16, step = 1, jump = 2,
							variable = { id = "OcclusionThreadpoolThreadCount", class = "TableVariable", table = mwseConfig },
							callback = clampOcclusionThreadCount,
							postCreate = captureThreadCountSlider,
						},
						{
							class = "Slider",
							label = i18n("occlusionThreadpoolBinsW.label"),
							description = i18n("occlusionThreadpoolBinsW.description"),
							min = 1, max = 16, step = 1, jump = 2,
							variable = { id = "OcclusionThreadpoolBinsW", class = "TableVariable", table = mwseConfig },
							callback = clampOcclusionThreadCount,
						},
						{
							class = "Slider",
							label = i18n("occlusionThreadpoolBinsH.label"),
							description = i18n("occlusionThreadpoolBinsH.description"),
							min = 1, max = 16, step = 1, jump = 2,
							variable = { id = "OcclusionThreadpoolBinsH", class = "TableVariable", table = mwseConfig },
							callback = clampOcclusionThreadCount,
						},
					},
				},
				{
					class = "Category",
					label = i18n("msoc.debugCategory.label"),
					components = {
						{
							class = "OnOffButton",
							label = i18n("debugOcclusionTintOccluder.label"),
							description = i18n("debugOcclusionTintOccluder.description"),
							variable = { id = "DebugOcclusionTintOccluder", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("debugOcclusionTintOccluded.label"),
							description = i18n("debugOcclusionTintOccluded.description"),
							variable = { id = "DebugOcclusionTintOccluded", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("debugOcclusionTintTested.label"),
							description = i18n("debugOcclusionTintTested.description"),
							variable = { id = "DebugOcclusionTintTested", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("occlusionLogPerFrame.label"),
							description = i18n("occlusionLogPerFrame.description"),
							variable = { id = "OcclusionLogPerFrame", class = "TableVariable", table = mwseConfig },
						},
						{
							class = "OnOffButton",
							label = i18n("occlusionLogAggregate.label"),
							description = i18n("occlusionLogAggregate.description"),
							variable = { id = "OcclusionLogAggregate", class = "TableVariable", table = mwseConfig },
						},
					},
				},
			},
			sidebarComponents = {
				{
					class = "Info",
					label = i18n("msoc.sidebarInfo.label"),
					text = i18n("msoc.sidebarInfo.text"),
				},
			},
		},
	},
	onClose = saveConfig,
}

local function registerModConfig()
	mwse.mcm.registerMCM(config)
end
event.register("modConfigReady", registerModConfig)
