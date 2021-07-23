return {
	description = "The combatStarted event occurs after combat has begun between two actors.",
	eventData = {
		["actor"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor who has entered combat.",
		},
		["target"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor who combat has been triggered against.",
		},
	},
	links = {
		["combatStart"] = "lua/event/combatStart",
	},
}