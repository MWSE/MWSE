return {
	type = "method",
	description = [[This method changes the active rendering target to the provided texture.]],
	arguments = {
		{ name = "texture", type = "niRenderedTexture", optional = true, description = "Set to `nil` to reset to the default rendering target." },
	},
	valuetype = "boolean",
}
