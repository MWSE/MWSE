return {
	type = "function",
	description = [[Creates a menu with a color picker. To read the color the user picked, pass a `closeCallback`.]],
	arguments = {
		{
			name = "params",
			type = "table",
			tableParams = {
				{ name = "id", type = "string|integer", optional = true, default = "MenuColorPicker", description = "The menu ID of the color picker menu." },
				{ name = "closeCallback", optional = true, type = "fun(selectedColor: mwseColorTable, selectedAlpha: number|nil)", description = "Called when the menu was closed. It gets passed the selected color and alpha values." },
				{ name = "initialColor", type = "mwseColorTable", description = "The initial color for the picker." },
				{ name = "alpha", type = "boolean", optional = true, default = false, description = "If `true` the picker will also allow picking an alpha value." },
				{ name = "initialAlpha", type = "number", optional = true, default = 1.0, description = "The initial alpha value." },
				{ name = "leaveMenuMode", type = "boolean", optional = true, default = false, description = "Determines if menu mode should be exited after a choice is made." },
				{ name = "heading", type = "string", optional = true, default = "Color Picker Menu", description = "The title of the opened menu. The default message is localized to the current locale." },
			},
		},
	}
}
