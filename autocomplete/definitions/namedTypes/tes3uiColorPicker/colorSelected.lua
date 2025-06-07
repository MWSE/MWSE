return {
	type = "method",
	description = "Changes the currently selected color. Updates current preview color, text input, saturation bar image and indicator positions.",
	arguments = {
		{ name = "newColor", type = "mwseColorTable|ffiImagePixel", description = "The new color to set." },
		{ name = "alpha", type = "number", optional = true, default = 1.0, description = "Alpha value to set." },
	}
}
