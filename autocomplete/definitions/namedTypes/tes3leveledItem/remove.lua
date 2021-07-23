return {
	type = "method",
	description = [[This function removes an item from the leveled list. True is returned if the operation was successful.]],
	arguments = {
		{ name = "entry", type = "tes3baseObject", description = "An item to remove." },
		{ name = "level", type = "number", description = "At which player level the item could be resolved from the leveled list." },
	},
	valuetype = "boolean",
}
