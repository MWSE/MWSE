return {
	type = "method",
	description = [[Creates an interactive button composed of images for the **idle**, **over**, and **pressed** states. The texture must have power-of-2 dimensions (i.e. 16, 32, 64, 128, 256, 512, 1024); the final display size can be trimmed by setting width and height.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "idle", type = "string", description = "The path to the idle image. This path is relative to ``Data Files``." },
		{ name = "over", type = "string", description = "The path to the mouse hover image. This path is relative to ``Data Files``." },
		{ name = "path", type = "string", description = "The path to the mouse pressed image. This path is relative to ``Data Files``." },
	},
    valuetype = "tes3uiElement",
}