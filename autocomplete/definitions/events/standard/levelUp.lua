return {
	description = "This even is called whenever the player successfully finishes leveling up.",
	eventData = {
		["level"] = {
			type = "number",
			readOnly = true,
			description = "The new level obtained.",
		},
	},
	filter = "level",
}