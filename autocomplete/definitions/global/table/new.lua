return {
	type = "function",
	description = [[This creates a pre-sized table. This is useful for big tables if the final table size is known and automatic table resizing is too expensive.]],
	arguments = {
		{ name = "narray", type = "number", description = "A hint for how many elements the array part of the table will have. Allocates fields for [0, narray]." },
		{ name = "nhash", type = "number", description = "A hint for how many elements the hash part of the table will have." },
	},
	returns = {
		{ name = "newTable", type = "table", description = "The pre-sized table that was created." }
	}
}