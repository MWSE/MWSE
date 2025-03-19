return {
	type = "function",
	description = [[Finds the closest exterior position to a reference, which will be a door exit from an interior cell to exterior cell, or just the reference's position in exteriors. It will search for the closest exterior to the player if no reference is given. The function recursively checks cells for connecting doors until an exterior is reached. Behave-as-exterior cells do not count as an exterior. If no exterior is reachable, nil will be returned.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", optional = true, type = "tes3reference", description = "The reference to search from. Defaults to the player reference if not provided." },
		},
	}},
	returns = {{ name = "position", type = "tes3vector3|nil" }},
}
