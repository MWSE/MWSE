return {
	type = "function",
	description = [[Returns an array-style table of all values in the given table, t. Optionally, it will sort the returned table.]],
	generics = {
		{ name = "valueType" }
	},
	arguments = {
		{ name = "t", type = "{ [unknown]: valueType }", description = "The table to get values for." },
		{ name = "sort", type = "boolean|fun(a: valueType, b: valueType):boolean", optional = true, description = "If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function." },
	},
	returns = {
		{ name = "values", type = "valueType[]", description = "An array of all table values." },
	},
}