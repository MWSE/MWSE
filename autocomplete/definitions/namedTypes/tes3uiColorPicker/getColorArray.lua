return {
	type = "method",
	description = "Gets the current RGB color in an array. Usually used to feed the color from the color picker straight to another UI element, e.g. `myElement.color = pickerElement.widget:getColorArray()`.",
	returns = {
		{ name = "arrayRGB", type = "number[]" },
	}
}
