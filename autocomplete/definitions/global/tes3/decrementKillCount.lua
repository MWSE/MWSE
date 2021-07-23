return {
	type = "function",
	description = [[Decreases player's kill count of a certain type of actor by one.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "actor", type = "tes3actor|string", description = "The actor (or their ID)." },
		},
	}},
}
