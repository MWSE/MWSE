return {
	type = "function",
	description = [[Sends all text input to the specified element.  Calling this function with a nil argument will release text input and allow keybinds to work. Suppresses most keybinds while active, except the Journal open/close keybind (it's up to the individual menu implementation).

Only one element can have control of input, and there is no automatic restoration of control if one element takes control from another. Be careful of conflicts with other users of this function.]],
	arguments = {
		{ name = "element", type = "tes3uiElement", optional = true },
	},
}
