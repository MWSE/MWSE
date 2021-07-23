return {
	type = "function",
	description = [[Changes a reference back from werewolf form to human. This function works only on a reference infected with Lycanthropy, be it the player or any other reference. Returns true if successful.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference", description = "A reference to change back to human." },
		},
	}},
	returns = "success",
	valuetype = "boolean",
}
