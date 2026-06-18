return {
	type = "function",
	description = "Shallow-copies values from one table to another table.",
	generics = {
		{ name = "fromType", inherits = "table" },
		{ name = "toType", inherits = "table" },
	},
	arguments = {
		{ name = "from", type = "fromType" },
		{ name = "to", type = "toType", optional = true },
	},
	valuetype = "fromType|toType",
}
