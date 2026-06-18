return {
	type = "function",
	description = "Splits a string using a Lua pattern. If the pattern has captures, the first capture from each match is stored; otherwise, the full match is stored.",
	arguments = {
		{ name = "inputString", type = "string" },
		{ name = "pattern", type = "string" },
	},
	valuetype = "string[]",
}
