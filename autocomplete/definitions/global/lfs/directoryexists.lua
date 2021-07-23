return {
	type = "function",
	description = [[Simple check to determine if a directory exists.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to determine existance of." },
	},
	returns = {
		{ name = "success", type = "boolean", description = "true if the directory exists." },
	},
}