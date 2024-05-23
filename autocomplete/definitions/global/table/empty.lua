return {
	type = "function",
	description = [[Checks if a table is empty.

If `deepCheck == true`, then tables are allowed to have nested subtables, so long as those subtables are empty. e.g., `table.empty({ {}, {} }, true) == true`, while `table.empty({ {}, {} }) == false`.]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "deepCheck", type = "boolean", optional = true, default = false, description = "If `true`, subtables will also be checked to see if they are empty." },
	},
	valuetype = "boolean",
}
