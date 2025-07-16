return {
	type = "function",
	description = [[Sets a value in a table and returns any previously defined value for that key.]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
		{ name = "key", type = "keyType", description = "The key to use to access the table." },
		{ name = "value", type = "any", description = "The value to set." },
	},
	returns = {
		{ name = "oldValue", type = "valueType|unknown|nil", description = "The previously defined value at `t[key]`."}
	}
}