return {
	description = "The simulate event occurs at the start of every frame, excluding when the game is paused or in menu mode.",
	eventData = {
		["delta"] = {
			type = "number",
			readOnly = true,
			description = "The number of seconds since the last frame.",
		},
		["timestamp"] = {
			type = "number",
			readOnly = true,
			description = "The number of hours since the beginning of the 3rd Era (similar to the Unix Epoch).",
		},
		["menuMode"] = {
			type = "boolean",
			readOnly = true,
			description = "Always false, maintained only for consistency with the enterFrame event.",
		},
	},
}
