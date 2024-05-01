return {
	type = "function",
	description = [[Gets font height metrics for a font.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "font", type = "number", optional = true, default = 0, description = "The index of the font." },
		}
	}},
	returns = {
		{ name = "maxGlyphHeight", type = "number", description = "Maximum pixel height of a single line of text." },
		{ name = "lineHeight", type = "number", description = "Pixel spacing between lines in a paragraph." },
	}
}
