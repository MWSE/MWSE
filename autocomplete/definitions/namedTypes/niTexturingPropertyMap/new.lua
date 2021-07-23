return {
	type = "function",
	description = [[Creates a new basic or bump map.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "texture", type = "niTexture", optional = true, description = "If provided, sets the map texture to the given value." },
			{ name = "clampMode", type = "number", default = "WRAP_S_WRAP_T", description = "The clamp mode to set the map to use." },
			{ name = "filterMode", type = "number", default = "TRILERP", description = "The filter mode to set the map to use." },
			{ name = "textCoords", type = "number", default = "0", description = "The texture coordinates to set the map to use." },
			{ name = "isBumpMap", type = "boolean", default = "false", description = "If true, a bump map will be created instead." },
		},
	}},
	returns = {
		{ name = "map", type = "niTexturingPropertyMap", description = "The created basic or bump map." },
	},
}