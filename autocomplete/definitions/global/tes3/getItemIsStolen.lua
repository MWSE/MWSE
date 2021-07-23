return {
	type = "function",
	description = [[This function checks item's stolen flag.]],
	arguments = {
		{ name = "item", type = "tes3item", description = "The item to check." },
		{ name = "from", type = "tes3baseObject", description = "Where the item was stolen from." }
	},
	returns = "isStolen",
	valuetype = "boolean",
}
