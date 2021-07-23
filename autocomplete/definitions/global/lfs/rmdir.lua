return {
	type = "function",
	description = [[Removes an existing directory. Unless using the recursive parameter, only empty directories can be removed.]],
	arguments = {
		{ name = "dirname", type = "string", description = "The name of the directory to remove." },
		{ name = "recursive", type = "boolean", default = "false", description = "If true, all contents of the directory will be removed as well." },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true in case of success, or nil on an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue." },
	},
}