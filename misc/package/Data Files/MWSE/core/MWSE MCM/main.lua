
local function saveConfig()
	local values = {}
	for k, _ in pairs(mwseConfig.getDefaults()) do
		values[k] = mwseConfig[k]
	end
	mwse.saveConfig("MWSE", values)
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
					label = "Log warnings with lua stacktrace?",
					class = "OnOffButton",
					description = "When enabled, any warnings raised during normal gameplay that would write to warnings.txt will also write to mwse.log. In addition, a stack trace is provided to give context to what mod may be responsible for the warning.\n\nThis can lead to superfluous log entries when using certain mods, like Better Bodies.",
					variable = {
						id = "LogWarningsWithLuaStack",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					label = "Run Morrowind in the background?",
					class = "OnOffButton",
					description = "By default Morrowind skips most game logic when not in focus. When using windowed mode (including borderless fullscreen), this feature allows Morrowind to continue running. Enabling this can also help to increase stability, due to how Morrowind handles some audio threads.",
					variable = {
						id = "RunInBackground",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					label = "Fix NiFlipController cloning?",
					class = "OnOffButton",
					description = "Morrowind incorrectly handles the affected map when cloning NiFlipController objects, preventing mods from using the controller for anything but the base map. MWSE fixes this issue. However, some mods contain bugged assets that have the incorrect affected map assigned, relying on the bug to reassign the controller back to the base map. Disabling this fix will provide support for these mods, but will also prevent newer mods from taking advantage of the fixed controller.\n\nIt is recommended that you leave this enabled unless you know you need the compatibility.",
					variable = {
						id = "PatchNiFlipController",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					label = "Cache all NetImmerse lua objects?",
					class = "OnOffButton",
					description = "WARNING: Only used for debugging purposes. Enabling this without knowing what you're doing WILL lead to memory leaks.\n\nWhen enabled, NetImmerse objects will use the same lua object caching system that TES3 objects use, allowing them to be string-compared or used as table keys.",
					variable = {
						id = "KeepAllNetImmerseObjectsAlive",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
				{
					label = "Enable legacy lua mods?",
					class = "OnOffButton",
					description = "If enabled, early alpha lua mods will be supported\n\nLegacy lua mods can be found in:\nData Files\\MWSE\\lua folder.",
					variable = {
						id = "EnableLegacyLuaMods",
						class = "TableVariable",
						table = mwseConfig,
					},
				},
			},
			sidebarComponents = {
				{
					label = "Notice:",
					class = "Info",
					text = "These options may not take effect until the next relaunch of the game.",
				},
				{
					label = "The following individuals have contributed to the Morrowind Script Extender:",
					class = "Info",
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
