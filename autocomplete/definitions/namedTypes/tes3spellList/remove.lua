return {
	type = "method",
	description = [[Removes a spell from the list. This is deprecated, please use tes3.removeSpell instead. This function does not update any modified flags or UI systems, so it is likely to cause issues.]],
	arguments = {
		{ name = "spell", type = "string|tes3spell", description = "The spell." },
	},
	valuetype = "boolean",
}