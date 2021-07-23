return {
	description = "The mouseButtonDown event fires when a button on the mouse is released.",
	eventData = {
		["button"] = {
			type = "number",
			readOnly = true,
			description = "The button index that was released.",
		},
		["isControlDown"] = {
			type = "number",
			readOnly = true,
			description = "True if control is held.",
		},
		["isAltDown"] = {
			type = "number",
			readOnly = true,
			description = "True if alt  is held.",
		},
		["isSuperDown"] = {
			type = "number",
			readOnly = true,
			description = "True if super (Windows key) is held.",
		},
	},
	filter = "button",
}