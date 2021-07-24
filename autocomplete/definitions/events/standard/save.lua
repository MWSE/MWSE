return {
	description = "The save event fires when the game is about to save. This event allows scripts to block saving.",
	related = { "save", "saved" },
	eventData = {
		["name"] = {
			type = "string",
			description = "The user facing name of the save, shown in the load/save game menu.",
		},
		["filename"] = {
			type = "string",
			description = "The filename of the save, not including file extension.",
		},
	},
	filter = "filename",
}