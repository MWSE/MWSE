return {
	description = "This event is raised when the player gains access to a new dialogue topic.",
	eventData = {
		["topic"] = {
			type = "tes3dialogue",
			readOnly = true,
			description = "The topic that was newly added to the player's list of topics.",
		},
	},
}