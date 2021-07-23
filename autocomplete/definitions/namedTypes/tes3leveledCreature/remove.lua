return {
	type = "method",
	description = [[This function removes a creature from the creature leveled list. True is returned if the operation was successful.]],
	arguments = {
		{ name = "entry", type = "tes3baseObject", description = "A creature to remove." },
		{ name = "level", type = "number", description = "At which player level the creature could be resolved from the leveled list." },
	},
	valuetype = "boolean",
}
