return {
	type = "method",
	description = [[Creates a new Color picker.]],
	arguments = {{
		name = "data",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "label", type = "string", optional = true, description = "Text shown on the top of the picker." },
			{ name = "description", type = "string", optional = true, description = "If in a [Sidebar Page](../types/mwseMCMSideBarPage.md), the description will be shown on mouseover." },
			{ name = "alpha", type = "boolean", optional = true, default = false, description = "If `true` the picker will also allow picking an alpha value." },
			{ name = "vertical", type = "boolean", optional = true, default = false, description = "If `true`, saturation, hue and alpha bars and color previews are created in the second row below the main picker. If `false` they are created in the same row as the main picker. Color picker is a large widget and as such, might not fit into a sidebar page or a filter page. It's useful to pass `vertical = true` to make it fit on those pages." },
			{ name = "variable", type = "mwseMCMVariable|mwseMCMSettingNewVariable", optional = true, description = "A variable for this Color picker." },
			{ name = "defaultSetting", type = "mwseColorTable|mwseColorATable", optional = true, description = "If `defaultSetting` wasn't passed in the `variable` table, can be passed here. The new variable will be initialized to this value." },
			{ name = "callback", type = "fun(self: mwseMCMColorPicker)", optional = true, description = "The custom function called when the player interacts with this Color picker." },
			{ name = "inGameOnly", type = "boolean", optional = true, default = false, description = "If true, the setting is disabled while the game is on main menu." },
			{ name = "restartRequired", type = "boolean", optional = true, default = false, description = "If true, updating this Setting will notify the player to restart the game." },
			{ name = "restartRequiredMessage", type = "string", optional = true, description = 'The message shown if restartRequired is triggered. The default text is a localized version of: "The game must be restarted before this change will come into effect."' },
			{ name = "indent", type = "integer", optional = true, default = 12, description = "The left padding size in pixels. Only used if the `childIndent` isn't set on the parent component." },
			{ name = "childIndent", type = "integer", optional = true, description = "The left padding size in pixels. Used on all the child components." },
			{ name = "paddingBottom", type = "integer", optional = true, default = 4, description = "The bottom border size in pixels. Only used if the `childSpacing` is unset on the parent component." },
			{ name = "childSpacing", type = "integer", optional = true, description = "The bottom border size in pixels. Used on all the child components." },
			{ name = "postCreate", type = "fun(self: mwseMCMColorPicker)", optional = true, description = "Can define a custom formatting function to make adjustments to any element saved in `self.elements`." },
			{ name = "class", type = "string", optional = true },
			{ name = "componentType", type = "string", optional = true },
			{ name = "parentComponent", type = "mwseMCMComponent", optional = true },
		}
	}},
	returns = {{
		name = "picker", type = "mwseMCMColorPicker"
	}}
}
