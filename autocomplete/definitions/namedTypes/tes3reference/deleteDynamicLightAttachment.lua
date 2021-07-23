return {
	type = "method",
	description = [[Deletes the dynamic light attachment, if it exists. This will automatically detach the dynamic light from affected nodes.]],
	arguments = {
		{ name = "removeLightFromParent", type = "boolean", default = "false", description = "If true, the dynamic light is removed from its parent node as well." },
	},
}