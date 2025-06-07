return {
	type = "method",
	description = "Changes the currently selected color. It calls `self:colorSelected` and then updates the main picker image. Because of that, it's more expensive.",
	arguments = {
		{ name = "newColor", type = "mwseColorTable|ffiImagePixel", description = "The new color to set." },
		{ name = "alpha", type = "number", optional = true, default = 1.0, description = "Alpha value to set." },
	}
}
