return {
	type = "function",
	description = [[This function will hide or unhide a mod in the MCM. The MCM must be closed and reopened for changes to take effect.]],
	arguments = {
		{ name = "modName", type = "string", description = "The name of the mod to hide or unhide." },
		{ name = "hidden", type = "boolean|nil", optional = true, description = "Whether to hide or unhide the mod. If `true`, the mod will be hidden. If `false`, the mod will be unhidden (i.e. shown). Default: `true`." },
	},
}