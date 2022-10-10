return {
	type = "function",
	description = [[Changes the 3rd person camera offset from the player's head.

!!! important
	This function can be used once tes3worldController and tes3mobilePlayer have finished initializing.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "offset", type = "tes3vector3", description = "The offset vector." },
		},
	}},
}
