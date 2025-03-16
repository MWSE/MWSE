return {
	type = "method",
	description = [[Determines if the enchantment contains an effect with the given id.]],
	arguments = {
		{ name = "effectId", type = "tes3.effect|integer", optional = false, description = "A value from [`tes3.effect`](https://mwse.github.io/MWSE/references/magic-effects/) table." },
	},
	returns = {
		{ name = "hasEffect", type = "boolean", description = "Returns `true` if the enchantment contains the given effect id, otherwise `false`." }
	},
}
