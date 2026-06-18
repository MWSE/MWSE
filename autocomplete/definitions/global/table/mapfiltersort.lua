return {
	type = "function",
	description = "Maps and filters an array, then sorts by the returned scores.",
	generics = {
		{ name = "valueType" },
	},
	arguments = {
		{ name = "array", type = "valueType[]" },
		{ name = "scoreFn", type = "fun(x: valueType): number|nil" },
	},
	returns = {
		{ name = "output", type = "valueType[]" },
		{ name = "scores", type = "number[]" },
	},
}
