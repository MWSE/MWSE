return {
	type = "function",
	description = [[This function check whether a reference is affected by a certain object or effect.

Note reference.object.spells:contains(spellID) will give the same output as this function for abilities, diseases, and curses, because having them in your spell list also makes them affect you.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string" },
			{ name = "effect", type = "tes3.effect", optional = true, description = "A tes3.effect constant to check for." },
			{ name = "object", type = "tes3alchemy|tes3enchantment|tes3spell|tes3magicEffect|string", optional = true, description = "An object to check for." },
		},
	}},
	returns = "isAffectedBy",
	valuetype = "boolean",
}
