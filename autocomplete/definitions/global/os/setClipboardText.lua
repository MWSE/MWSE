return {
	type = "function",
	description = [[Sets the current clipboard text. If nil is passed, the clipboard will be cleared instead. If either operation fails, the function returns false.]],
	arguments = {
		{ name = "text", type = "string", optional = true, description = "The text to put in the clipboard. If nil is provided instead, then the clipboard will be cleared." },
	},
	valuetype = "boolean",
}