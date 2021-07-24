return {
	description = "The combatStopped event occurs after an actor has left combat.",
	related = { "combatStart", "combatStarted", "combatStop", "combatStopped" },
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