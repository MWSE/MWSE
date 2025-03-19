return {
	type = "function",
	description = [[Performs word wrapping of text.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "text", type = "string", description = "The text to wrap." },
			{ name = "font", type = "number", optional = true, default = 0, description = "The index of the font." },
			{ name = "maxWidth", type = "number", optional = true, default = -1, description = "The wrapping width in pixels." },
			{ name = "ignoreLinkDelimiters", type = "boolean", optional = true, default = false },
		},
	}},
	returns = {
		{ name = "wrappedText", type = "string", description = "The wrapped text, with `\\n` as line breaks." },
		{ name = "lineCount", type = "number", description = "The number of lines in the output wrapped text." },
	}
}
