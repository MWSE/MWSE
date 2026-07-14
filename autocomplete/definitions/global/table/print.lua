return {
	type = "function",
	description = "Prints the result of `table.getprintabletable(inputTable, ...)`.",
	arguments = {
		{ name = "inputTable", type = "table|userdata" },
		{ name = "maxDepth", type = "integer", optional = true },
		{ name = "indentStr", type = "string", optional = true },
		{ name = "indentLevel", type = "integer", optional = true },
	},
}
