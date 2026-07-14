return {
	type = "function",
	description = [[Moves current toward target by at most step, without overshooting.]],
	arguments = {
		{ name = "current", type = "number" },
		{ name = "target", type = "number" },
		{ name = "step ", type = "number", description = "Positive step expected." },
	},
	valuetype = "number",
}
