return {
	type = "function",
	description = "Returns a printable multi-line representation of a table-like value.",
	arguments = {
		{ name = "inputTable", type = "table|userdata" },
		{ name = "maxDepth", type = "integer", optional = true, default = 50 },
		{ name = "indentStr", type = "string", optional = true, description = "Defaults to a tab character." },
		{ name = "indentLevel", type = "integer", optional = true, default = 0 },
	},
	valuetype = "string",
}
