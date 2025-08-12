return {
	type = "function",
	description = [[Gets all the loggers registered to a particular `modDir`.]],
	arguments = {
		{ name = "modDir", type = "string",  description = [[The directory of mod to retrieve the loggers for. This argument corresponds to the `lua-mod` field of the mods metadata file.]] }
	},
	returns = {
		{ name = "loggers", type = "mwseLogger[]|nil", description = "The loggers, if they exist." }
	}
}
