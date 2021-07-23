return {
	type = "method",
	description = [[Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.]],
	arguments = {
		{ name = "texture", type = "niTexture", optional = true, description = "The texture to assign to the new decal." },
	},
	returns = {
		{ name = "map", type = "niTexturingPropertyMap|nil", description = "A newly created decal map." },
		{ name = "index", type = "number|nil", description = "The index of the newly added decal map." },
	},
}