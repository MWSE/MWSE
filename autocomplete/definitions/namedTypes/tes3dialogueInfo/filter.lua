return {
	type = "method",
	description = "This method filters the associated dialogue info for given arguments and returns true if the dialogue info filtering passes. This method rises infoFilter event.",
	arguments = {
		{ name = "actor", type = "tes3object", },
		{ name = "reference", type = "tes3reference", },
		{ name = "source", type = "number", },
		{ name = "dialogue", type = "tes3dialogue", },
	},
	returns = "result",
	valuetype = "boolean",
}
