return {
	type = "method",
	description = [[Log warn message.]],
	arguments = {
		{ name = "message", type = "string" },
		{ name = "...", type = "any", description = "Formatting arguments. These are passed to `string.format`.", optional = true },
	}
}
