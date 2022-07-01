return {
	type = "event",
	description = "The cellChanged event is triggered when the player changes cells. This might occur from going through a door, using intervention or recall spells, or from scripted repositioning.",
	related = { "cellActivated", "cellDeactivated", "cellChanged" },
	eventData = {
		["cell"] = {
			type = "tes3cell",
			readOnly = true,
			description = "The new cell that the player has entered.",
		},
		["previousCell"] = {
			type = "tes3cell",
			readOnly = true,
			description = "The previous cell that the player came from. This will be nil when loading a game.",
		},
	},
	filter = "cell",
}