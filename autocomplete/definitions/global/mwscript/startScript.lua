return {
	type = "function",
	description = [[Use [`tes3.runLegacyScript()`](https://mwse.github.io/MWSE/apis/tes3/#tes3runlegacyscript) instead. Wrapper for the `StartCombat` mwscript function. Starts the script as a global script.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "The target reference for this command to be executed on. Defaults to the normal script execution reference.", optional = true },
			{ name = "script", type = "tes3script|string" },
		},
	}},
	returns = {{ name = "executed", type = "boolean" }},
}
