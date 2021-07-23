return {
	type = "function",
	description = [[Sets player's kill count of a certain type of actor.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "actor", type = "tes3actor|string", description = "The actor (or their ID) for whom to set player's kill count." },
			{ name = "count", type = "number", description = "Number of kills that will be set." },
		},
	}},
}
