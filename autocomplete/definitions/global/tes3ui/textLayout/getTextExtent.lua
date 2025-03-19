return {
	type = "function",
	description = [[Calculates expected size information for text content.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "text", type = "string", description = "The text to use." },
			{ name = "font", type = "number", optional = true, default = 0, description = "The index of the font." },
			{ name = "firstLineOnly", type = "boolean", optional = true, default = false, description = "Only process the first line of the text." },
		},
	}},
	returns = {
		{ name = "width", type = "number", description = "Pixel width of the widest line of the text." },
		{ name = "height", type = "number", description = "Pixel height of a label containing this text. Includes the extra space and rounding added by the label layout." },
		{ name = "verticalAdvance", type = "number", description = "The vertical displacement that a following text element would use. It is zero for text without newlines, and increases with each newline." },
	}
}
