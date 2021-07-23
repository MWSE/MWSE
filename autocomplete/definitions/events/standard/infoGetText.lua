return {
	description = "This event fires when a dialogue info object text is retrieved.",
	eventData = {
		["info"] = {
			type = "tes3dialogueInfo",
			readOnly = true,
			description = "The dialogue info object.",
		},
		["loadOriginalText"] = {
			type = "method",
			description = "Loads the original text for the info from the game files, bypassing this event.",
		},
	},
}