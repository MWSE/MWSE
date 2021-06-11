return {
	type = "method",
	description = [[Fetches the effect of the spell in whose spell school the actor is least proficient in.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "npc", type = "tes3npc", description = "The actor to perform check against." },
		},
	}},
	valuetype = "tes3effect",
}
