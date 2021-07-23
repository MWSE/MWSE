return {
	description = "The combatStopped event occurs after an actor has left combat.",
	eventData = {
		["actor"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor who has left combat.",
		},
	},
	links = {
		["combatStop"] = "lua/event/combatStop",
	},
}