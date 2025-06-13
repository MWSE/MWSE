return {
	type = "function",
	description = [[Gets a value in a table. If the key doesn't exist in the table, a specified default value will be set in the table and returned instead.]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
		{ name = "key", type = "keyType", description = "The key to use to access the table." },
		{ name = "defaultValue", type = "valueType", description = "The default value to set and return if the key didn't exist in the table." },
	},
	returns = "valueType",
}