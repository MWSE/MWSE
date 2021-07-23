return {
	type = "function",
	description = [[Removes an item from a given reference's inventory.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "Who to remove items from." },
			{ name = "item", type = "tes3item|string", description = "The item to remove." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data for the exact item to remove." },
			{ name = "deleteItemData", type = "tes3itemData", optional = true, description = "Whether to delete the item data after remove succeeds. Automatically set if itemData is used. Does not need to be specified for normal usage." },
			{ name = "count", type = "number", default = "1", description = "The maximum number of items to remove." },
			{ name = "playSound", type = "boolean", default = true, description = "If false, the up/down sound for the item won't be played." },
			{ name = "updateGUI", type = "boolean", default = true, description = "If false, the function won't manually resync the player's GUI state. This can result in some optimizations, though `tes3ui.forcePlayerInventoryUpdate()` must manually be called after all inventory updates are finished." },
		},
	}},
	returns = {{ name = "removedCount", type = "number" }},
}
