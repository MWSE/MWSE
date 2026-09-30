return {
	type = "function",
	description = [[Fetches the core game region object for a given region ID. If the region with a given ID doesn't exist, nil is returned.]],
	arguments = {{ name = "id", type = "string", description = "ID of the region to search for." }},
	returns = {{ name = "region", type = "tes3region?" }}
}
