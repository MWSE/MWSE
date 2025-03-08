return {
	type = "function",
	description = [[Zooms out by the specified amount, or by a small amount if no amount is provided.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "amount", type = "number", default = 0.0625 },
		},
	}},
}
