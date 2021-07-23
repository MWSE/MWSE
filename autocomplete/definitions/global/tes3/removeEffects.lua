return {
	type = "function",
	description = [[Removes magic effects from a given reference. Requires that either the effect or castType parameter be provided.]],
	arguments = {
		{ name = "reference", type = "tes3reference", description = "Target reference to remove effects from." },
		{ name = "effect", type = "number", optional = true, description = "Maps to tes3.effect constants." },
		{ name = "castType", type = "number", optional = true, description = "Maps to tes3.castType constants." },
		{ name = "chance", type = "number", default = 100, description = "The chance for the effect to be removed." },
		{ name = "removeSpell", type = "boolean", optional = true, description = "If removing by cast type, determines if the spell should be removed from the target's spell list. Defaults to true if castType is not tes3.castType.spell. This causes diseases and curses to be removed when dispelled." },
	},
}