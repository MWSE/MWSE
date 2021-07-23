return {
	type = "method",
	description = [[Draws map marker boxes onto the world map for all cells with names starting with the prefix. e.g. showLocationOnMap("Vivec") draws boxes for all cells in Vivec, because the cell names "Vivec, Arena", "Vivec, Temple", etc. matches.]],
	arguments = {
		{ name = "cellNamePrefix", type = "string" },
	},
}