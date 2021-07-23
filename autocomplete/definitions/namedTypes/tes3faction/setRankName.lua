return {
	type = "method",
	description = [[This function changes the player-facing name of a given rank in the faction.]],
	arguments = {
		{ name = "rank", type = "number", description = "A rank number in range [0, 9] inclusive." },
		{ name = "name", type = "string", description = "A new name to set." },
	},
}
