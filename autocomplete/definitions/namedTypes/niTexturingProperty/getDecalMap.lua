return {
	type = "method",
	description = [[Finds an existing decal applied to the property.]],
	arguments = {
		{ name = "textureOrFileName", type = "niTexture|string", description = "The texture or file name of the source texture to find." },
	},
	returns = {
		{ name = "map", type = "niTexturingPropertyMap|nil", description = "The existing map." },
		{ name = "index", type = "integer|nil", description = "The index of the existing decal map." },
	},
}