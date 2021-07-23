return {
	type = "function",
	description = [[This function sets the owner of a reference.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "A reference whose owner to set."},
			{ name = "remove", type = "boolean", optional = true, default = false, description = "If this parameter is set to true, reference's owner field will be removed." },
			{ name = "owner", type = "tes3npc|tes3faction|string", description = "Assigns this NPC or a faction as the owner of the reference." },
			{ name = "requiredGlobal", type = "tes3globalVariable", optional = true, description = "If owner is set to NPC, requiredGlobal variable can be set." },
			{ name = "requiredRank", type = "number", optional = true, default = 0, description = "If owner is set to faction, requitedRank variable controls minimal rank in faction the player has to have to be able to freely take the reference." },
		},
	}},
}
