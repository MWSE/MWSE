return {
	description = "The menuExit event fires when the player leaves menu mode.",
	related = { "menuEnter", "menuExit" },
	eventData = {
		["menuMode"] = {
			type = "boolean",
			readOnly = true,
			description = "Always true for this event. This parameter is only useful when registering the same callback for menuEnter and menuExit events.",
		},
	},
}