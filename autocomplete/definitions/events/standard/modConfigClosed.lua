return {
	type = "event",
	description = 
		"This event fires when a mod config menu has been closed. \z
			This event fires whenever a different mod config menu is selected, or whenever the MCM itself is closed.\n\n\z
		\z
		You can use this event to update your mod whenever its MCM is closed. \z
			This event may also be useful if you're trying to make your mod compatible with another mod, \z
			as it lets you detect whenever that mod updates its settings.\z
	",
	eventData = {
		["modName"] = {
			type = "string",
			readOnly = true,
			description = "The name of the mod that was closed.",
		},
		["isFavorite"] = {
			type = "boolean",
			readOnly = true,
			description = "Whether that mod was a favorite.",
		},
	},
	filter = "modName",
	related = { "modConfigReady"},

}