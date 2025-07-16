return {
	type = "function",
	description = [[Returns an array-style table of all keys in the given table, t. Optionally, it will sort the returned table.]],
	generics = {
		{ name = "keyType" }
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: unknown }", description = "The table to get keys for." },
		{ name = "sort", type = "boolean|fun(a: keyType, b: keyType): boolean", optional = true, description = "If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function." },
	},
	returns = {
		{ name = "keys", type = "keyType[]", description = "An array of all table keys." },
	},
}