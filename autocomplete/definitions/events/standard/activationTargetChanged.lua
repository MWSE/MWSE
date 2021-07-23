return {
	description = "This event is raised when the potential activation target for the player changes.",
	eventData = {
		["current"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The activation target for the player, should they press the activation key.",
		},
		["previous"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The previous activation target.",
		},
	},
	filter = "current",
}