return {
	type = "function",
	description = [[This function changes an item's stolen flag. Morrowind handles stealing by marking the base item (not the inventory stack) with with NPCs that you have stolen that item from. The NPC will recognize an item is stolen if they are marked as stolen on the base item.]],
	arguments = {
		{ name = "item", type = "tes3item", description = "The item whose stolen flag to modify." },
		{ name = "from", type = "tes3baseObject", description = "The location the item is stolen from." },
		{ name = "stolen", type = "boolean", optional = true, default = true, description = "If this parameter is set to true, the item will be flagged as stolen. Otherwise, the item's stolen flag will be removed." },
	},
}
