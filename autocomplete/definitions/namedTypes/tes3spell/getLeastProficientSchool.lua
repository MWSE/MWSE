return {
	type = "method",
	description = [[Fetches the magic school of the spell in which the actor is least proficient in.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "npc", type = "tes3npc", description = "The actor to perform check against." },
		},
	}},
	valuetype = "tes3.magicSchool constants",
}
