return {
	type = "function",
	description = [[This creates a pre-sized table. This is useful for big tables if the final table size is known and automatic table resizing is too expensive.]],
	arguments = {
		{ name = "narray", type = "number", description = "A hint for how many elements the table will have as a sequence." },
		{ name = "nhash", type = "number", description = "A hint for how many other elements the table will have." },
	},
	returns = {
		{ name = "newTable", type = "table", description = "The pre-sized table that was created." }
	}
}