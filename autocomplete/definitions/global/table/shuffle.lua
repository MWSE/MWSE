return {
	type = "function",
	description = [[Shuffles the table in place using the Fisher-Yates algorithm. Passing in table size as the second argument saves the function from having to get it itself.]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "n", type = "integer", optional = true, default = "#t", description = "The length of the array." },
	},
}
