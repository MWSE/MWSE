return {
	type = "function",
	description = [[Fetches the core game region object for a given region ID. If the region with a given ID doesn't exist, nil is returned.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string", description = "ID of the region to search for." },
		},
	}},
	returns = "region",
	valuetype = "tes3region",
}
