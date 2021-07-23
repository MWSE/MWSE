return {
	type = "method",
	description = [[This function inserts given creature in the creature leveled list. True is returned if the operation was successful.]],
	arguments = {
		{ name = "entry", type = "tes3baseObject", description = "A creature to insert." },
		{ name = "level", type = "number", description = "At which player level the creature can be resolved from the leveled list." },
	},
	valuetype = "boolean",
}
