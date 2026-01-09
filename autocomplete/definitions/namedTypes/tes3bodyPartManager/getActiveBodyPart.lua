return {
	type = "method",
	description = [[The method fetches the active body part at a given layer and position. This gives access to the associated scene node, equipped item, and base body part.]],
	arguments = {
		{ name = "layer", type = "tes3.activeBodyPartLayer", description = "A value from [`tes3.activeBodyPartLayer`](https://mwse.github.io/MWSE/references/active-body-part-layers/) namespace." },
		{ name = "index", type = "tes3.activeBodyPart", description = "A value from [`tes3.activeBodyPart`](https://mwse.github.io/MWSE/references/active-body-parts/) namespace." },
	},
	valuetype = "tes3bodyPartManagerActiveBodyPart",
	examples = {
		["example"] = {
			title = "Getting an active body part structure for the currently equipped weapon"
		},
		["changing position"] = {
			title = "Adjusting the position of the currently equipped iron dagger on the player character"
		}
	}
}
