return {
	type = "function",
	description = [[Using the same engine function used by the vanilla intervention spells, locations the first reference of a given object in the exterior world.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "object", type = "tes3physicalObject|string", description = "The object to search for." },
			{ name = "position", optional = true, type = "tes3vector3", description = "The position to search from. Must be an exterior position. Defaults to the last exterior position of the player if no position is provided." },
		},
	}},
	returns = {{ name = "reference", type = "tes3reference" }},
}
