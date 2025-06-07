return {
	type = "event",
	description = "The menuExit event fires when the player leaves menu mode.",
	related = { "menuEnter", "menuExit" },
	eventData = {
		["menuMode"] = {
			type = "boolean",
			readOnly = true,
			description = "Always false for this event, indicating that menu mode is off. This parameter is only useful when registering the same callback for menuEnter and menuExit events.",
		},
	},
}