return {
	type = "value",
	description = [[The eight gamepad state slots, indexed from 1 to 8. Only the first gamepadCount slots are active. Zero-based gamepad ID n in events and helpers corresponds to gamepadStates[n + 1]. Includes both gamepads and joysticks enumerated by vanilla DirectInput.]],
	readOnly = true,
	valuetype = "tes3gamepadState[]",
}
