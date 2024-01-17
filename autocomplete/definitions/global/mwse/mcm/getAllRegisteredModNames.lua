return {
	type = "function",
	description = [[Returns a list containing the names of all mods that have been registered to the MCM.]],
	arguments = {
		{ name = "sort", type = "boolean|function|nil", optional = true, description = "If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function." },
	},
	returns = {
		{ name = "modNames", type = "table", description = "An array containing the names of all mods that have been registered to the MCM." },
	},
}