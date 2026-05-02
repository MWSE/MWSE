return {
	type = "event",
	description = "The key event fires when a key up or key down input is detected. It is preferred that the keyDown and keyUp events are used instead.",
	eventData = {
		["keyCode"] = {
			type = "tes3.scanCode",
			readOnly = true,
			description = "The scan code of the key that raised the event. Maps to values in [`tes3.scanCode`](https://mwse.github.io/MWSE/references/scan-codes/) table.",
		},
		["pressed"] = {
			type = "boolean",
			readOnly = true,
			description = "True if this is a key down event, false for a key up event.",
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
	examples = {
		["PrintCtrlZ"] = {
			title = "Show a Message when Ctrl-Z is Pressed",
			description = "Displays a simple message when Z is pressed while control is held.",
		},
	},
}