return {
	type = "function",
	description = [[Wrapper for the RemoveFromLevCreature mwscript function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "list", type = "tes3leveledCreature|string", description = "Leveled creature list to remove a creature from." },
			{ name = "creature", type = "tes3actor|string", description = "Creature to remove from the list." },
			{ name = "level", type = "number", default = 0, description = "Minimum level for the creature to spawn." },
		},
	}},
	returns = {{ name = "executed", type = "boolean" }},
}
