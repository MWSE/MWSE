return {
	type = "function",
	description = [[Returns an array-style table of all keys in the given table, t. Optionally, it will sort the returned table.]],
	arguments = {
		{ name = "t", type = "table", description = "The table to get keys for." },
		{ name = "sort", type = "boolean|function|nil", optional = true, description = "If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function." },
	},
	returns = {
		{ name = "keys", type = "table", description = "An array of all table keys." },
	},
}