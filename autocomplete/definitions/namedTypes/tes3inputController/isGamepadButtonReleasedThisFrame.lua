return {
	type = "method",
	description = [[Returns whether a gamepad button changed from held to released in the latest input poll. Returns false for an out-of-range gamepad or button ID.]],
	arguments = {
		{ name = "gamepad", type = "integer", description = "Zero-based gamepad ID, from 0 to gamepadCount - 1." },
		{ name = "button", type = "integer", description = "Zero-based DirectInput button ID, from 0 to 31. Button mappings depend on the device." },
	},
	valuetype = "boolean",
}
