return {
	type = "function",
	description = [[Returns a copy of `t` with the keys and values flipped.]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
	},
	valuetype = "{ [valueType]: keyType }",
}
