return {
	type = "function",
	description = [[Returns the key for a given value, or `nil` if the table does not contain the value.]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
		{ name = "value", type = "valueType|any" },
	},
	returns = {
		{ name = "key", type = "keyType|nil", description = "A `key` such that `tbl[key] == value`, if such a key exists. `nil` otherwise." }
	},
}