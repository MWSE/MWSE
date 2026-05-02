return {
	type = "event",
	description = "The key event fires when a key is released.",
	related = { "keyDown" },
	eventData = {
		["keyCode"] = {
			type = "tes3.scanCode",
			readOnly = true,
			description = "The scan code of the key that raised the event. Maps to values in [`tes3.scanCode`](https://mwse.github.io/MWSE/references/scan-codes/) table.",
		},
		["isShiftDown"] = {
			type = "boolean",
			readOnly = true,
			description = "True if either shift key is held.",
		},
		["isControlDown"] = {
			type = "boolean",
			readOnly = true,
			description = "True if either control key is held.",
		},
		["isAltDown"] = {
			type = "boolean",
			readOnly = true,
			description = "True if either alt key is held.",
		},
		["isSuperDown"] = {
			type = "boolean",
			readOnly = true,
			description = "True if super (Windows key) is held.",
		},
		["element"] = {
			type = "tes3uiElement?",
			readOnly = true,
			description = "The UI element that the key is going to be dispatched to, or `nil` if one is not in focus.",
		},
	},
	filter = "keyCode",
}