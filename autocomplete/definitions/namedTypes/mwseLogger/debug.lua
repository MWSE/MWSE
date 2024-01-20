return {
	type = "method",
	description = [[Log debug message.]],
	arguments = {
		{ name = "message", type = "string" },
		{ type = "variadic", description="Formatting arguments. These are passed to `string.format`.", optional = true }
	}
}
