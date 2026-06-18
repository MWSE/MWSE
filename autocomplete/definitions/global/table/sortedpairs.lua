return {
	type = "function",
	description = "Returns an iterator over keys in sorted order.",
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "tbl", type = "{ [keyType]: valueType }" },
		{ name = "comparator", type = "fun(a: keyType, b: keyType): boolean", optional = true },
	},
	returns = {
		{ name = "iterator", type = "fun(): keyType, valueType" },
	},
}
