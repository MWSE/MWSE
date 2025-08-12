return {
	type = "method",
	description = [[Logs and raises an error if the value of its argument v is false (i.e., `nil` or `false`); otherwise, returns all its arguments.]],
	generics = {
		{ name = "T" },
	},
	arguments = {
		{ name = "v", type = "T", optional = true },
		{ name = "message", type = "string|fun(...): ..." },
		{ name = "...", type = "any", description = "Formatting arguments. These are passed to `string.format`. Tables and userdata values are pretty printed.", optional = true },
	},
	returns = {
		{ name = "v", type = "T" },
		{ name = "...", type = "any" },
	}
}
