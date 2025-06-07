return {
	type = "method",
	description = [[Gets the first index of an effect ID in the spell effect table. Returns `-1` if provided effect doesn't exist in the spell.]],
	arguments = {
		{ name = "effectId", type = "tes3.effect|integer", optional = false, description = "A value from [`tes3.effect`](https://mwse.github.io/MWSE/references/magic-effects/) table." },
	},
	returns = {
		{ name = "index", type = "integer", description = "Returns 0-based index. Because Lua's arrays are 1-based, to index the spell's `effects` array with the return value add 1." }
	},
}
