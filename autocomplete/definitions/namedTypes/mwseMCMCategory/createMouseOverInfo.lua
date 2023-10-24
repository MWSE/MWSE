return {
	type = "method",
	description = [[Creates a new nested MouseOverInfo.]],
	arguments = {{
		name = "data",
		type = "table|string",
		optional = true,
		description = "If passing only a string, it will be used as the MouseOverInfo's label.",
		tableParams = {
			{ name = "label", type = "string", optional = true, description = "The MouseOverInfo's label." },
			{ name = "text", type = "string", optional = true, description = "The MouseOverInfo's text." },
			{ name = "description", type = "string", optional = true, description = "If in a [Sidebar Page](../types/mwseMCMSideBarPage.md), the description will be shown on mouseover." },
			{ name = "variable", type = "mwseMCMVariable|mwseMCMSettingNewVariable", optional = true, description = "A variable for this setting." },
			{ name = "defaultSetting", type = "unknown", optional = true, description = "If `defaultSetting` wasn't passed in the `variable` table, can be passed here. The new variable will be initialized to this value." },
			{ name = "inGameOnly", type = "boolean", optional = true, default = false },
			{ name = "indent", type = "integer", optional = true, default = 12, description = "The left padding size in pixels. Only used if the `childIndent` isn't set on the parent component." },
			{ name = "childIndent", type = "integer", optional = true, description = "The left padding size in pixels. Used on all the child components." },
			{ name = "paddingBottom", type = "integer", optional = true, default = 4, description = "The bottom border size in pixels. Only used if the `childSpacing` is unset on the parent component." },
			{ name = "childSpacing", type = "integer", optional = true, description = "The bottom border size in pixels. Used on all the child components." },
			{ name = "postCreate", type = "fun(self: mwseMCMMouseOverInfo)", optional = true, description = "Can define a custom formatting function to make adjustments to any element saved in `self.elements`." },
		}
	}},
	returns = {{
		name = "info", type = "mwseMCMMouseOverInfo"
	}}
}