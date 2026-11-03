return {
	type = "event",
	description = [[Fires after vanilla input polling when a gamepad button is released. Includes joysticks enumerated by vanilla DirectInput and requires the game to initialize those devices. This observes raw state changes, including state resets when acquisition fails. It does not block game input.]],
	related = { "gamepadButtonDown", "gamepadButtonUp", "gamepadAxis", "gamepadPov" },
	eventData = {
		["gamepad"] = { type = "integer", readOnly = true, description = "The zero-based gamepad ID." },
		["button"] = { type = "integer", readOnly = true, description = "The zero-based DirectInput button ID, from 0 to 31. Device mappings vary." },
	},
	filter = "gamepad",
}
