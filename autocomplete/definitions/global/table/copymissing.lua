return {
	type = "function",
	description = [[Copies a table's contents from one table to another, including subtitles. If a non-table key is already defined, it will not be overwritten. Metatables are not copied.]],
	arguments = {
		{ name = "to", type = "table" },
		{ name = "from", type = "table" },
	},
}