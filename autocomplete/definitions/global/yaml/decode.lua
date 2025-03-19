return {
	type = "function",
	description = [[Decode string representing yaml into a table.]],
	arguments = {
		{ name = "input", type = "string", description = "The string to be decoded into a table." },
	},
	returns = {
		{ name = "result", type = "table", description = "The decoded table." },
	},
}
