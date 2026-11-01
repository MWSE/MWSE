return {
	type = "event",
	description = [[Fires after vanilla input polling when a gamepad POV hat changes. Includes joysticks enumerated by vanilla DirectInput and requires the game to initialize those devices. This observes raw state changes, including state resets when acquisition fails. It does not block game input.]],
	related = { "gamepadButtonDown", "gamepadButtonUp", "gamepadAxis", "gamepadPov" },
	eventData = {
		["gamepad"] = { type = "integer", readOnly = true, description = "The zero-based gamepad ID." },
		["pov"] = { type = "integer", readOnly = true, description = "The zero-based POV hat ID, from 0 to 3." },
		["isCentered"] = { type = "boolean", readOnly = true, description = "True when the POV hat is centered (no direction is pressed)." },
		["wasCentered"] = { type = "boolean", readOnly = true, description = "True when the POV hat was centered in the preceding input poll." },
		["value"] = { type = "number", readOnly = true, description = "Clockwise angle in degrees, from 0 up to but not including 360. North is 0, east is 90, south is 180, and west is 270. When isCentered is true, this is 0 and does not represent a direction." },
		["previousValue"] = { type = "number", readOnly = true, description = "The clockwise angle in degrees from the preceding input poll. When wasCentered is true, this is 0 and does not represent a direction." },
	},
	filter = "gamepad",
}
