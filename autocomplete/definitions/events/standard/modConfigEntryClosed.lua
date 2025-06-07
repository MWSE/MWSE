return {
	type = "event",
	description = 
		"This event fires when a particular mod's configuration menu has been closed. \z
			This happens when the MCM is closed or when a different mod's configuration menu was opened.\n\n\z
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