return {
	description = "The key event fires when a key is released.",
	eventData = {
		["keyCode"] = {
			type = "number",
			readOnly = true,
			description = "The scan code of the key that raised the event.",
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
	filter = "keyCode",
}