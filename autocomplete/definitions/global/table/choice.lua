return {
	type = "function",
	description = [[Returns a random element from the given table.]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
	},
	returns = {
		{ name = "value", type = "valueType", description = "The randomly chosen value." },
		{ name = "key", type = "keyType", description = "The table key of the chosen value." }
	}
}