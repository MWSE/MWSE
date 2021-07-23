return {
	description = "This event is raised when determining the hit chance for an actor.",
	eventData = {
		["attacker"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the mobile's reference.",
		},
		["attackerMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile who is making the attack.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the target mobile's reference. May not always be available.",
		},
		["targetMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile who is being attacked. May not always be available.",
		},
		["hitChance"] = {
			type = "number",
			description = "The hit chance for the actor. This may be adjusted.",
		},
	},
	filter = "attacker.baseObject",
}