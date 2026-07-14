return {
	type = "function",
	description = [[Returns true when raw table storage has no non-nil values.

This low-level helper inspects raw table storage directly. It does not honor `__pairs`, readonly proxy backing storage, or `table.makereadonly` backing contents.]],
	arguments = {
		{ name = "t", type = "table" },
	},
	valuetype = "boolean",
}
