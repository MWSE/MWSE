return {
	type = "event",
	description = "The menuEnter event fires when the player enters menu mode.",
	related = { "menuEnter", "menuExit" },
	eventData = {
		["menu"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The new top-level menu.",
		},
		["menuMode"] = {
			type = "boolean",
			readOnly = true,
			description = "Always true for this event, indicating that menu mode is on. This parameter is only useful when registering the same callback for menuEnter and menuExit events.",
		},
	},
	filter = "menu.name",
}