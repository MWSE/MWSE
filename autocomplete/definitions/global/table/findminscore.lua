return {
	type = "function",
	description = "Finds the array element with the lowest score returned by `scoreFn`.",
	generics = {
		{ name = "valueType" },
	},
	arguments = {
		{ name = "array", type = "valueType[]" },
		{ name = "scoreFn", type = "fun(x: valueType): number|nil" },
	},
	returns = {
		{ name = "element", type = "valueType|nil", description = "The element with the lowest score." },
		{ name = "score", type = "number|nil", description = "The lowest score." },
		{ name = "index", type = "integer|nil", description = "The index of the element with the lowest score." },
	},
}
