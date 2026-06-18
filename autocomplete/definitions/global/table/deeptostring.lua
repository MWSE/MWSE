return {
	type = "function",
	description = "Recursively converts a value to a human-readable string.",
	arguments = {
		{ name = "value", type = "any" },
		{ name = "level", type = "integer", optional = true, default = 1, description = "The maximum recursion depth." },
		{ name = "prefix", type = "string", optional = true, description = "The current indentation prefix." },
	},
	valuetype = "string",
}
