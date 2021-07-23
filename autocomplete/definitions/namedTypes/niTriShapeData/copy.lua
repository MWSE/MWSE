return {
	type = "method",
	description = [[Creates a copy of the data. An optional table of filters can be passed to remove information from the copy.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "normals", type = "boolean", default = "true", description = "If false, the geometry data's normals will be absent from the copy." },
			{ name = "colors", type = "boolean", default = "true", description = "If false, the geometry data's colors will be absent from the copy." },
			{ name = "texCoords", type = "boolean", default = "true", description = "If false, the geometry data's texture coordinates will be absent from the copy." },
		},
	}},
	returns = {
		{ name = "copiedData", type = "niTriShapeData", description = "The duplicated data." },
	},
}