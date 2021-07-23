return {
	type = "function",
	description = [[Returns the effect of the spell that a given actor is least proficient with.]],
	arguments = {	
		{ name = "actor", type = "tes3mobileActor|tes3reference|tes3npc|tes3npcInstance", optional = false, description = "The actor to calculate for. A mobile actor is preferred, as passing only a tes3npc will not use up to date skill information." },
    },
	returns = {
		{ name = "effect", type = "tes3effect|nil", description = "The least proficient effect, or nil if the spell has no valid effects." },
	}
}
