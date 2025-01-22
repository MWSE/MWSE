return {
	type = "function",
	description = [[Use [`tes3.getLegacyScriptRunning()`](https://mwse.github.io/MWSE/apis/tes3/#tes3getlegacyscriptrunning) instead. Wrapper for the `ScriptRunning` mwscript function. Only checks global scripts.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "script", type = "tes3script|string" },
		},
	}},
	returns = "boolean",
}
