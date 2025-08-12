return {
	type = "function",
	description = [[Shallowly copies a table's contents to a destination table. If no destination table is provided, a new table will be created. Note that sub tables will not be copied, and will still refer to the same data.]],
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