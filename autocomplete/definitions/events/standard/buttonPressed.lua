return {
	description = "The buttonPressed event is unique in that it is invoked only when using tes3.messageBox to present buttons to the player.",
	eventData = {
		["button"] = {
			type = "number",
			readOnly = true,
			description = "The button index, 0-based, that was pressed.",
		},
	},
}