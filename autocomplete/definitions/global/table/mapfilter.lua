return {
	type = "function",
	description = "Maps an array through `scoreFn`, keeping values with non-false scores.",
	generics = {
		{ name = "valueType" },
		{ name = "scoreType" },
	},
	arguments = {
		{ name = "array", type = "valueType[]" },
		{ name = "scoreFn", type = "fun(x: valueType): scoreType|nil" },
	},
	returns = {
		{ name = "output", type = "valueType[]" },
		{ name = "scores", type = "scoreType[]" },
	},
}
