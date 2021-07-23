return {
	type = "function",
	description = [[Toggles the camera into third person mode. Returns true if changed to vanity mode.

Note that unlike the vanity mode caused by not doing anything for a while, this vanity mode must be toggled to go off.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "enabled", type = "boolean", optional = true, default = true, description = "" },
			{ name = "checkVanityDisabled", type = "boolean", optional = true, default = true, description = "" },
			{ name = "toggle", type = "boolean", optional = true, default = false, description = "" },
		},
	}},
	returns = "changedVanityMode",
	valuetype = "boolean",
}
