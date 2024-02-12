return {
	type = "method",
	description = [[Returns the index of the closest scale key with timing less than or equal to given `time` argument.]],
	arguments = {
		{ name = "time", type = "number" },
	},
	returns = {
		{ name = "lastKeyIndex", type = "integer|nil" },
	},
}
