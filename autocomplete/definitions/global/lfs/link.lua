return {
	type = "function",
	description = [[Creates a link. This is a hard link by default.]],
	arguments = {
		{ name = "old", type = "string", description = "The object to link to." },
		{ name = "new", type = "string", description = "The name of the link." },
		{ name = "symlink", type = "boolean", optional = true, description = "If true, the link will be symbolic instead." },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true in case of success, or nil on an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue." },
	},
}