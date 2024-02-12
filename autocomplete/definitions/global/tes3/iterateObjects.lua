return {
	type = "function",
	description = [[Iteration function used for looping over game objects.]],
	arguments = {
		{ name = "filter", type = "integer|integer[]", optional = true, description = "Maps to [`tes3.objectType`](https://mwse.github.io/MWSE/references/object-types/) constants." }
	},
	returns = {
		name = "objectIterator",
		type = "fun(): tes3object"
	}
}
