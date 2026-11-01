return {
	type = "event",
	description = [[Fires after vanilla input polling when a gamepad axis or slider changes. Includes joysticks enumerated by vanilla DirectInput and requires the game to initialize those devices. This observes raw state changes, including state resets when acquisition fails. It does not block game input.]],
	related = { "gamepadButtonDown", "gamepadButtonUp", "gamepadAxis", "gamepadPov" },
	eventData = {
		["gamepad"] = { type = "integer", readOnly = true, description = "The zero-based gamepad ID." },
		["axis"] = { type = "tes3.gamepadAxis", readOnly = true, description = "The zero-based axis ID. Compare against tes3.gamepadAxis constants." },
		["value"] = { type = "integer", readOnly = true, description = "Raw absolute device-dependent axis value. No additional normalization or dead zone is applied." },
		["previousValue"] = { type = "integer", readOnly = true, description = "The value from the preceding input poll, in the same units as value." },
	},
	filter = "gamepad",
}
