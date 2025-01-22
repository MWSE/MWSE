return {
	type = "function",
	description = [[This function returns true if a mwscript is currently running. Only checks global scripts.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "script", type = "tes3script|string", description = "The script to check for." },
		},
	}},
	returns = {{ name = "isRunning", type = "boolean" }},
}
