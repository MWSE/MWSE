return {
	type = "function",
	description = [[Use [`tes3reference:delete()`](https://mwse.github.io/MWSE/types/tes3reference/#delete) instead. Marks the `reference` as deleted, and modified. Deleted reference isn't rendered nor is its local mwscript run.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "The target reference for this command to be executed on. Defaults to the normal script execution reference.", optional = true },
			{ name = "delete", type = "boolean", optional = true, default = "true", description = "Setting this to true deletes the reference and triggers `referenceDeactivated` event. Setting this to false effectively undeletes/activates the reference and triggers `referenceActivated` event." },
		},
	}},
	returns = {{ name = "executed", type = "boolean" }},
}
