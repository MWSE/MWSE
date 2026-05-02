return {
	type = "method",
	description = [[Creates a new nested LogLevelOptions setting. This is a specialized dropdown with a default label and description text provided for convenience when using MWSE's logger.]],
	arguments = {{
		name = "data",
		type = "table",
		tableParams = {
			{ name = "label", type = "string", optional = true, description = "The text shown above the dropdown. If not provided, the default label text will be used." },
			{ name = "description", type = "string", optional = true, description = "If in a [Sidebar Page](../types/mwseMCMSideBarPage.md), the description will be shown on mouseover. If not provided, the default description will be used." },
			{ name = "logger", type = "mwseLogger|string", optional = true, description = "A logger whose logging level will be changed. The setting will try to automatically resolve your mod's logger using the same mechanism as `mwse.Logger.new`. If this isn't possible, an error message will be printed to the `MWSE.log`. In such a case, you can pass your mod's logger or mod name." },
			{ name = "options", type = "mwseMCMDropdownOption[]", optional = true, description = "This table holds the text and variable value for each of the dropdown's options." },
			{ name = "variable", type = "mwseMCMVariable|mwseMCMSettingNewVariable", optional = true,
				description = "A variable for this setting. If not provided, this setting will try to create a variable using the `config` and `configKey` parameters, if possible."
			},
			{ name = "config", type = "table", optional = true, default = "`parentComponent.config`",
				description = "The config to use when creating a [`mwseMCMTableVariable`](./mwseMCMTableVariable.md) for this `Setting`. \z
						If provided, it will override the config stored in `parentComponent`. \z
						Otherwise, the value in `parentComponent` will be used."
			},
			{ name = "defaultConfig", type = "table", optional = true, default = "`parentComponent.defaultConfig`",
				description = "The `defaultConfig` to use when creating a [`mwseMCMTableVariable`](./mwseMCMTableVariable.md) for this `Setting`. \z
					If provided, it will override the `defaultConfig` stored in `parentComponent`. \z
					Otherwise, the value in `parentComponent` will be used."
			},
			{ name = "configKey", type = "string|number", optional = true,
				description = "The `configKey` used to create a new [`mwseMCMTableVariable`](./mwseMCMTableVariable.md). \z
					If this is provided, along with a `config` (which may be inherited from the `parentComponent`), then a new \z
						[`mwseMCMTableVariable`](./mwseMCMTableVariable.md) variable will be created for this setting."
			},
			{ name = "converter", type = "fun(newValue: unknown): unknown", optional = true,
				description = "A converter to use for this component's `variable`."
			},
			{ name = "defaultSetting", type = "unknown", optional = true,
				description = "If `defaultSetting` wasn't passed in the `variable` table, can be passed here. \z
					The new variable will be initialized to this value. If not provided, then the value in `defaultConfig` will be used, if possible."
			},
			{ name = "showDefaultSetting", type = "boolean", optional = true, default = "`parentComponent.showDefaultSetting`",
				description = "If `true`, and in a [Sidebar Page](../types/mwseMCMSideBarPage.md), then the `defaultSetting` of this setting's `variable` will be shown below its `description`. \z
					The `defaultSetting` will be formatted in accordance with the `convertToLabelValue` function. \z
					**Note:** This parameter does not update the `description` field.",
			},
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
		}
	}},
	returns = {{
		name = "dropdown", type = "mwseMCMLogLevelOptions"
	}}
}
