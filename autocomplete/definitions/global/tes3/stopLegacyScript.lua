return {
	type = "function",
	description = [[This function stops a global mwscript.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "script", type = "tes3script|string", description = "The script to stop." },
		},
	}},
}
