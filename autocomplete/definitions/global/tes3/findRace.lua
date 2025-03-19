return {
	type = "function",
	description = [[Fetches the core game character race object for a given ID. If the race with a given ID doesn't exist, nil is returned.]],
	arguments = {
		{ name = "id", type = "string", description = "ID of the race to search for." },
	},
	returns = {
		{ name = "race", type = "tes3race?" },
	},
}
