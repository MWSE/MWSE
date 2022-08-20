return {
	type = "function",
	description = [[Determines if a reference has access to another object, including its inventory. References have access to their own things, and the player has access to dead NPC's items.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", optional = true, default = "tes3.player", description = "The actor to check permissions for.", },--See what the default value ends up in the meta file
			{ name = "target", type = "tes3reference|tes3mobileActor|string", description = "The reference to check access of.", },
		},
	}},
	returns = "hasAccess",
	valuetype = "boolean",
}