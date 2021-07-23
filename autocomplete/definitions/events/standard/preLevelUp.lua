return {
	description = "This event is triggered just before the player levels up.",
	eventData = {
		["level"] = {
			type = "number",
			readOnly = true,
			description = "The new level about to be obtained.",
		},
	},
	filter = "level",
}