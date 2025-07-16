return {
	type = "function",
	description = [[Gets a logger associated with a particular `modDir`. You can also filter by `filepath` as well.]],
	arguments = {
		{ name = "modDir", type = "string",  description = [[The directory of mod to retrieve the loggers for. This argument corresponds to the `lua-mod` field of the mod's metadata file.]] },
		{ name = "filepath", type = "string",  optional = true, description = [[The filepath to retrieve the logger for. This is relative to `modDir` and corresponds to the `filepath` field of the logger to retrieve.]] }
	},
	returns = {
		{ name = "logger", type = "mwseLogger|nil", description = "The logger, if it exists." }
	}
}
