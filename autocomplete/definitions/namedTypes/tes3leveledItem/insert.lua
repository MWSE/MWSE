return {
	type = "method",
	description = [[This function inserts given item in the leveled list. True is returned if the operation was successful.]],
	arguments = {
		{ name = "entry", type = "tes3baseObject", description = "An item to insert." },
		{ name = "level", type = "number", description = "At which player level the item can be resolved from the leveled list." },
	},
	valuetype = "boolean",
}
