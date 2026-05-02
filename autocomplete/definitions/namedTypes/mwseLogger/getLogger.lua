return {
	type = "function",
	deprecated = true,
	description = [[Gets a logger with a specific `modName`. You should use `mwseLogger.get` instead.]],
	arguments = {
		{ name = "modName", type = "string",  description = "The name of the mod to retrieve the logger for."}
	},
	returns = {
		{ name = "logger", type = "mwseLogger|nil", description = "The logger, if it exists." }
	}
}
