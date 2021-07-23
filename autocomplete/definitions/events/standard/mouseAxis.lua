return {
	description = "The mouseAxis event fires when the mouse is moved, providing deltaX and deltaY values.",
	eventData = {
		["deltaX"] = {
			type = "number",
			readOnly = true,
			description = "The difference in horizontal mouse position from the previous frame.",
		},
		["deltaY"] = {
			type = "number",
			readOnly = true,
			description = "The difference in vertical mouse position from the previous frame.",
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
}