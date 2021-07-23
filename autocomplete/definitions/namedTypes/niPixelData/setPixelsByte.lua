return {
	type = "method",
	description = [[Sets the pixel data from byte data.]],
	arguments = {
		{ name = "data", type = "table", description = "The byte data to be set to." },
		{ name = "mipMapLevel", type = "number", description = "Default value: 1", optional = true },
	},
}