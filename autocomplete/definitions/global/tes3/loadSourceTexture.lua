return {
	type = "function",
	description = [[Loads a source texture file and provides the niSourceTexture object.]],
	arguments = {
		{ name = "path", type = "string", description = "Path, relative to Data Files/Textures." },
		{ name = "useCache", type = "boolean", default = true, description = "If false, a new object will be created even if it had been previously loaded." },
	},
	returns = "texture",
	valuetype = "niSourceTexture",
}