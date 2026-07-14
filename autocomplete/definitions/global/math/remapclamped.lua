return {
	type = "function",
	description = "Remaps a value from one range to another, then clamps to the output range.",
	arguments = {
		{ name = "value", type = "number" },
		{ name = "lowIn", type = "number" },
		{ name = "highIn", type = "number" },
		{ name = "lowOut", type = "number" },
		{ name = "highOut", type = "number" },
	},
	valuetype = "number",
}