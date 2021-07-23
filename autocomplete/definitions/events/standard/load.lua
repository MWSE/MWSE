return {
	description = "The load event fires when the game is about to load. This event allows scripts to block loading.",
	eventData = {
		["filename"] = {
			type = "string",
			description = "The filename of the save we want to load, not including file extension. Can be modified to redirect to another save.",
		},
		["quickload"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, the load came from the quickload system.",
		},
		["newGame"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, a new game is being started.",
		},
	},
	filter = "filename",
}