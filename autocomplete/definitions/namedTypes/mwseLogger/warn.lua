return {
	type = "method",
	description = [[Log warn message.]],
	arguments = {
		{ name = "message", type = "string" },
		{ type = "variadic", description="Formatting arguments. These are passed to `string.format`.", optional = true }
	}
}
