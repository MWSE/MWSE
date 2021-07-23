return {
	type = "method",
	description = [[Creates an image element from a texture file. The texture must have power-of-2 dimensions (i.e. 16, 32, 64, 128, 256, 512, 1024); the final display size can be trimmed by setting width and height.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "path", type = "string", description = "An image path. This path is relative to ``Data Files``." },
	},
    valuetype = "tes3uiElement",
}