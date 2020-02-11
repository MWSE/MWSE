return {
	type = "function",
	description = [[Wrapper for the RemoveFromLevItem mwscript function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "list", type = "tes3leveledItem|string", description = "Leveled item list to remove an item from." },
			{ name = "item", type = "tes3item|string", description = "Item to remove from the list." },
			{ name = "level", type = "number", default = 0, description = "Minimum level for the item to spawn." },
		},
	}},
	returns = {{ name = "executed", type = "boolean" }},
}
