return {
	type = "function",
	description = "This clears all keys and values from a table, but preserves the allocated array/hash sizes. This is useful when a table, which is linked from multiple places, needs to be cleared and/or when recycling a table for use by the same context. This avoids managing backlinks, saves an allocation and the overhead of incremental array/hash part growth.\n\nPlease note this function is meant for very specific situations. In most cases it's better to replace the (usually single) link with a new table and let the garbage collection do its work.",
	arguments = {
		{ name = "table", type = "table", description = "The table to clear." },
	},
}