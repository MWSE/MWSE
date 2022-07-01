return {
	type = "function",
	description = [[This function changes an item's stolen flag. Morrowind handles stealing by marking the base item (not the inventory stack) with NPCs that you have stolen that item from. The NPC will recognize an item as stolen if they are marked as stolen on the base item.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3item", description = "The item whose stolen flag to modify." },
			{ name = "from", type = "tes3creature|tes3npc|tes3faction|nil", description = "Who or what to set/clear the stolen state for. If not provided, the stolen state can be cleared (but not set) for all objects." },
			{ name = "stolen", type = "boolean", default = true, description = "If this parameter is set to true, the item will be flagged as stolen. Otherwise, the item's stolen flag will be removed." },
		},
	}},
}
