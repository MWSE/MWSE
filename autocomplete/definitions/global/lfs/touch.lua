return {
	type = "function",
	description = [[Set access and modification times of a file. Both access and modified times are measured in seconds, matching the OS time function.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to touch." },
		{ name = "accessTime", type = "number", optional = true, description = "The new access time to set." },
		{ name = "modifiedTime", type = "number", optional = true, description = "The new last modified time to set" },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true in case of success, or nil on an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue." },
	},
}