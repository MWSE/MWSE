return {
	description = "This event fires when an actor has determined an action in a combat session.",
	eventData = {
		["session"] = {
			type = "tes3combatSession",
			readOnly = true,
			description = "The combat session the action has been determined for.",
		},
	},
}