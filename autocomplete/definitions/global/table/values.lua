return {
	type = "function",
	description = [[Returns an array-style table of all values in the given table, t. Optionally, it will sort the returned table.]],
	arguments = {
		{ name = "t", type = "table", description = "The table to get values for." },
		{ name = "sort", type = "boolean|function|nil", optional = true, description = "If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function." },
	},
	returns = {
		{ name = "values", type = "table", description = "An array of all table values." },
	},
}