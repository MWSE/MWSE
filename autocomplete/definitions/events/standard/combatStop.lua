return {
	description = "The combatStop event occurs when an actor is about to leave combat. This event allows scripts to prevent combat from stopping.",
	eventData = {
		["actor"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor who is exiting combat.",
		},
	},
}