return {
	type = "function",
	description = [[Fetches the core game birthsign object for a given birthsign ID. If the birthsign with a given ID doesn't exist, nil is returned.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string", description = "ID of the birthsign to search for." },
		},
	}},
	returns = "birthsign",
	valuetype = "tes3birthsign",
}
