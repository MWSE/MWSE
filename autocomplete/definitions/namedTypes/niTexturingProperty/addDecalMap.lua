return {
	type = "method",
	description = [[Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.]],
	arguments = {
		{ name = "texture", type = "niTexture", optional = true, description = "The texture to assign to the new decal." },
		{ name = "priority", type = "number", default = 0.0, description = "The desired sorting priority of the decal. Whenever a new decal is added, decals are resorted to give priority to decals with a higher number. Only 8 textures will ever be rendered at a time, leaving lower priority decals to potentially go unseen." },
		{ name = "allowDuplicates", type = "boolean", default = false, description = "If true, duplicates of the same texture can be added. Normally, an existing decal of the same texture will be returned instead." },
	},
	returns = {
		{ name = "map", type = "niTexturingPropertyMap|nil", description = "A newly created decal map." },
		{ name = "index", type = "integer|nil", description = "The index of the newly added decal map." },
	},
}