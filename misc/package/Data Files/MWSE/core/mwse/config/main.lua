local function saveConfig()
	local values = {}
	for k, _ in pairs(mwseConfig.getDefaults()) do
		values[k] = mwseConfig[k]
	end
	mwse.saveConfig("MWSE", values)
end

local i18n = mwse.loadTranslations("mwse.config")

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
			label = "SideBar Page",
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
	},
	onClose = saveConfig,
}

local function registerModConfig()
	mwse.mcm.registerMCM(config)
end
event.register("modConfigReady", registerModConfig)
