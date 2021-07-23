return {
	type = "function",
	description = [[Changes a reference to werewolf form. This function works only on a reference infected with Lycanthropy, be it the player or any other reference. Returns true if successful.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference", description = "A reference to change to werewolf form." },
		},
	}},
	returns = "success",
	valuetype = "boolean",
}
