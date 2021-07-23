return {
	type = "function",
	description = [[Increases player's kill count of a certain type of actor by one.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "actor", type = "tes3actor|string", description = "The actor (or their ID) for whom to increase player's kill count." },
		},
	}},
}
