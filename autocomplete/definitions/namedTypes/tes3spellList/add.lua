return {
	type = "method",
	deprecated = true,
	description = [[Adds a spell to the list. This is deprecated, please use tes3.addSpell instead. This function does not update any modified flags or UI systems, so it is likely to cause issues.]],
	arguments = {
		{ name = "spell", type = "string|tes3spell", description = "The spell." },
	},
	valuetype = "boolean",
}