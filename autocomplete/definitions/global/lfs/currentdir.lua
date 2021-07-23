return {
	type = "function",
	description = [[Changes the current working directory to the given path.]],
	returns = {
		{ name = "path", type = "string|nil", description = "The current working directory, or nil on an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue." },
	},
}