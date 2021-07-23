return {
	type = "method",
	description = [[Attempts to remove a decal at a given index.]],
	arguments = {
		{ name = "index", type = "number", description = "The index of the decal to remove." },
	},
	returns = {
		{ name = "removed", type = "boolean", description = "True if the decal was removed." },
	},
}