return {
	type = "method",
	description = "Changes the color of this widget to given color.",
	arguments = {
		{ name = "newColor", type = "mwseColorTable|ffiImagePixel", description = "The new color to set." },
		{ name = "alpha", type = "number", optional = true, default = 1.0, description = "Alpha value to set." },
	}
}
