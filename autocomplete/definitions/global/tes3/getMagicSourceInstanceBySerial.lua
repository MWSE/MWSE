return {
	type = "function",
	description = [[Fetches an instance of the magic source of a given serial number.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "serialNumber", type = "number" },
		},
	}},
	returns = "magicSourceInstance",
	valuetype = "tes3magicSourceInstance",
}
