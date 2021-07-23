return {
	type = "function",
	description = [[Returns how many times the player killed an actor. If no actor is specified, total number of kills player commited will be returned.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "actor", type = "tes3actor|string", optional = true, description = "The actor (or their ID) for whom to retrieve player's kill count." },
		},
	}},
    returns = "count",
    valuetype = "number",
}
