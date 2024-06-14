return {
	type = "function",
	description = [[This function checks whether a certain key combination is currently pressed. It will only check ctrl, shift and alt modifier keys. It doesn't check mouse.]],
	arguments = {
		{ name = "keybind", type = "mwseKeyCombo" },
	},
	returns = { name = "pressed", type = "boolean" }
}