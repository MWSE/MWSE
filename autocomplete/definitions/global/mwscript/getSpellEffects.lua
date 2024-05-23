return {
	type = "function",
	description = [[Use [`tes3.isAffectedBy()`](https://mwse.github.io/MWSE/apis/tes3/#tes3isaffectedby) or [`tes3mobileActor:isAffectedByObject()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#isaffectedbyobject) instead. Wrapper for the `GetSpellEffects` mwscript function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "The target reference for this command to be executed on. Defaults to the normal script execution reference.", optional = true },
			{ name = "spell", type = "tes3spell|string" },
		},
	}},
	returns = "boolean",
}
