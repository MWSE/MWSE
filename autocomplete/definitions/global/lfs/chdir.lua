return {
	type = "function",
	description = [[Changes the current working directory to the given path. Note that this is global. Do not use this function without restoring the previous directory at the end of your logic. Failure to do so can cause unrecoverable errors.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to change to." },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true in case of success, or nil on an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue." },
	},
}