return {
	type = "function",
	description = [[Checks if a table is empty.

This is a compatibility helper that uses `pairs` and therefore respects normal iteration behavior such as `__pairs`. By contrast, `table.isempty` is a lower-level rubic0n helper that inspects raw table storage and does not honor `__pairs` or readonly proxy backing storage.

If `deepCheck == true`, then tables are allowed to have nested subtables, so long as those subtables are empty. e.g., `table.empty({ {}, {} }, true) == true`, while `table.empty({ {}, {} }) == false`. Use `table.isempty` when you specifically need a raw-storage emptiness check.]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "deepCheck", type = "boolean", optional = true, default = false, description = "If `true`, subtables will also be checked to see if they are empty." },
	},
	valuetype = "boolean",
}
