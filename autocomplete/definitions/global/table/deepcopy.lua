return {
	type = "function",
	description = [[Copies a table's contents. All subtables will also be copied, as will any metatable.]],
	generics = {
		{ name = "tableType", inherits = "table" }
	},
	arguments = {
		{ name = "t", type = "tableType" },
	},
	valuetype = "tableType",
}