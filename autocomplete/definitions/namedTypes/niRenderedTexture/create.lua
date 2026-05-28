return {
	type = "function",
	description = [[Creates an niRenderedTexture.]],
	arguments = {
		{ name = "width", type = "number", description = "The width of the rendered texture. Should be a power of 2." },
		{ name = "height", type = "number", description = "The height of the rendered texture. Should be a power of 2." },
		{ name = "prefs", type = "niFormatPrefs", optional = true, description = "Custom preferences to use. By default, the texture uses 32-bit true color, no mipmaps, and smooth alphas." },
	},
	returns = { name = "texture", type = "niRenderedTexture" }
}