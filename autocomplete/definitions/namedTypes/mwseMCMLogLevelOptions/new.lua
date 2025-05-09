return {
	type = "method",
	description = [[Creates a new LogLevelOptions setting.]],
	arguments = {{
		name = "data",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "label", type = "string", optional = true, description = "The text shown above the dropdown." },
			{ name = "description", type = "string", optional = true, description = "If in a [Sidebar Page](../types/mwseMCMSideBarPage.md), the description will be shown on mouseover." },
			{ name = "logger", type = "mwseLogger|string", optional = true, description = "A logger whose logging level will be changed. The setting will try to automatically resolve your mod's logger using the same mechanism as `mwse.Logger.new`. If this isn't possible, an error message will be printed to the `MWSE.log`. In such a case, you can pass your mod's logger or mod name." },
			{ name = "options", type = "mwseMCMDropdownOption[]", optional = true, description = "This table holds the text and variable value for each of the dropdown's options." },
			{ name = "variable", type = "mwseMCMVariable|mwseMCMSettingNewVariable", optional = true, description = "A variable for this setting." },
			{ name = "defaultSetting", type = "unknown", optional = true, description = "If `defaultSetting` wasn't passed in the `variable` table, can be passed here. The new variable will be initialized to this value." },
			{ name = "idleColor", type = "number[]", optional = true, default = "tes3ui.getPalette(tes3.palette.normalColor)", description = "The idle color for dropdown. Needs to be an RGB trio in the range [0.0, 1.0]." },
			{ name = "overColor", type = "number[]", optional = true, default = "tes3ui.getPalette(tes3.palette.normalOverColor)", description = "The color used when the mouse if hovering over the dropdown. Needs to be an RGB trio in the range [0.0, 1.0]." },
			{ name = "pressedColor", type = "number[]", optional = true, default = "tes3ui.getPalette(tes3.palette.normalPressedColor)", description = "The color used when the dropdown is being pressed. Needs to be an RGB trio in the range [0.0, 1.0]." },
			{ name = "callback", type = "fun(self: mwseMCMLogLevelOptions)", optional = true, description = "The custom function called when the player interacts with this Setting." },
			{ name = "inGameOnly", type = "boolean", optional = true, default = false, description = "If true, the setting is disabled while the game is on main menu." },
			{ name = "restartRequired", type = "boolean", optional = true, default = false, description = "If true, updating this Setting will notify the player to restart the game." },
			{ name = "restartRequiredMessage", type = "string", optional = true, description = 'The message shown if restartRequired is triggered. The default text is a localized version of: "The game must be restarted before this change will come into effect."' },
			{ name = "indent", type = "integer", optional = true, default = 12, description = "The left padding size in pixels. Only used if the `childIndent` isn't set on the parent component." },
			{ name = "childIndent", type = "integer", optional = true, description = "The left padding size in pixels. Used on all the child components." },
			{ name = "paddingBottom", type = "integer", optional = true, default = 4, description = "The bottom border size in pixels. Only used if the `childSpacing` is unset on the parent component." },
			{ name = "childSpacing", type = "integer", optional = true, description = "The bottom border size in pixels. Used on all the child components." },
			{ name = "postCreate", type = "fun(self: mwseMCMLogLevelOptions)", optional = true, description = "Can define a custom formatting function to make adjustments to any element saved in `self.elements`." },
			{ name = "class", type = "string", optional = true },
			{ name = "componentType", type = "string", optional = true },
			{ name = "parentComponent", type = "mwseMCMComponent", optional = true },
		}
	}},
	returns = {{
		name = "dropdown", type = "mwseMCMLogLevelOptions"
	}}
}
