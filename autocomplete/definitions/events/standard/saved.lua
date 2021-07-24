return {
	description = "The saved event fires after the game has successfully been saved. This event is preceded by the save event.",
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
}