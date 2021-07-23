return {
	type = "function",
	description = [[Creates a new directory.]],
	arguments = {
		{ name = "dirname", type = "string", description = "The name of the new directory." },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true in case of success, or nil on an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue." },
	},
}