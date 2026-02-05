return {
	type = "method",
	description = [[Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.]],
	arguments = {
		{ name = "texture", type = "niTexture", optional = true, description = "The texture to assign to the new decal." },
		{ name = "index", type = "number", optional = true, description = "The desired index to store the decal in, for sorting purposes. Defaults to `ni.texturingPropertyMapType.decalLast`. Note that added decals are compacted down to remove gaps, so the actual index will differ." },
	},
	returns = {
		{ name = "map", type = "niTexturingPropertyMap|nil", description = "A newly created decal map." },
		{ name = "index", type = "integer|nil", description = "The index of the newly added decal map." },
	},
}