return {
	type = "function",
	description =
	[[This function performs a check whether the provided reference is affected by a certain object or magic effect.

!!! info
	`reference.object.spells:contains(spellID)` will give the same output as this function for abilities, diseases, and curses, because having them in your spell list also makes them affect you.

!!! tip
	Soul trap effect is only active on the actor during the frame of the actor's death, if the player has a valid soul gem.
]],
	arguments = { {
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string" },
			{ name = "effect",    type = "tes3.effect|integer",                                          optional = true, description = "A numerical identifier of the magic effect to perform a check for. Maps to [`tes3.effect`](https://mwse.github.io/MWSE/references/magic-effects/) constant, including those claimed with `tes3.claimSpellEffectId()`, and then added with `tes3.addMagicEffect()`." },
			{ name = "object",    type = "tes3alchemy|tes3enchantment|tes3spell|tes3magicEffect|string", optional = true, description = "An object to perform a check for." },
		},
	} },
	returns = "isAffectedBy",
	valuetype = "boolean",
}
