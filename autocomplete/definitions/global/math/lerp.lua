return {
	type = "function",
	description = [[Performs linear interpolation between values v0 and v1. Returns a value that is t percent between them.]],
	arguments = {
		{ name = "v0", type = "number", description = "First value." },
		{ name = "v1", type = "number", description = "Second value." },
		{ name = "t", type = "number", description = "The decimal percentage used to calculate a point between v0 and v1." },
	},
	valuetype = "number",
}
