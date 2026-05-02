return {
	type = "method",
	description = [[Modifies the value of a given crime statistic.]],
	arguments = {
		{ name = "key", type = "string", description = "The key for the crime statistic." },
		{ name = "delta", type = "number", description = "The change in value for the statistic." },
	},
	returns = {
		{ name = "newValue", type = "number" },
	},
}
