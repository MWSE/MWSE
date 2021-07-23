return {
	type = "method",
	description = [[Creates a NIF model from a file. Still under research.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "text", type = "string", description = "A model path. This path is relative to ``Data Files``." },
	},
    valuetype = "tes3uiElement",
}