return {
	type = "function",
	description = [[Creates an niSourceTexture from the given filepath.]],
	arguments = {
		{ name = "path", type = "string", description = "The filepath of the texture to load. Relative to `Data Files\\`." },
		{ name = "useCached", type = "boolean", optional = true, default = true, description = "If true, the texture will be stored in the normal texture source cache, so that multiple calls to the same path will return the same object. This behavior can be disabled if necessary." },
	},
	returns = { name = "texture", type = "niSourceTexture" }
}