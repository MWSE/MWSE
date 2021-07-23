return {
	type = "function",
	description = [[This function sets a reference's animation groups' timings to a specified value.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "A reference whose animation groups' timings to set." },
			{ name = "timing", type = "number|table", description = "If one parameter is provided, all animation data timings will be set to the timing provided. If a table with 3 timings is provided, animation data timings will be set accordingly." },
		},
	}},
}
