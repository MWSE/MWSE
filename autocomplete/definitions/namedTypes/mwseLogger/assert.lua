return {
	type = "method",
	description = [[Assert a condition and log an error if it fails.]],
	arguments = {
		{ name = "condition", type = "boolean" },
		{ name = "message", type = "string" },
		{ name = "...", type = "any", description = "Formatting arguments. These are passed to `string.format`.", optional = true },
	}
}
