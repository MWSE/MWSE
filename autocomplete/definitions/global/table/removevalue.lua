return {
	type = "function",
	description = [[Removes a `value` from a given `list`. Returns `true` if the value was successfully removed.]],
	generics = {
		{ name = "valueType" }
	},
	arguments = {
		{ name = "list", type = "{ [unknown]: valueType }" },
		{ name = "value", type = "valueType" },
	},
	valuetype = "boolean",
}