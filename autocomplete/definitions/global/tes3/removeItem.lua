return {
	type = "function",
	description = [[Removes an item from a given reference's inventory. Items without itemData will be removed first. The `reference` will be cloned if needed.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "Who to remove items from." },
			{ name = "item", type = "tes3item|string", description = "The item to remove." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data for the exact item to remove." },
			{ name = "count", type = "number", optional = true, default = "1", description = "The maximum number of items to remove." },
			{ name = "playSound", type = "boolean", optional = true, default = true, description = "If false, the up/down sound for the item won't be played." },
			{ name = "reevaluateEquipment", type = "boolean", optional = true, default = true, description = "If true, and the item removed is armor, clothing, or a weapon, the actor will reevaluate its equipment choices to see if it needs to equip a new item. This does not affect the player." },
			{ name = "updateGUI", type = "boolean", optional = true, default = true, description = "If false, the function won't manually resync the player's GUI state. This can result in some optimizations, though [`tes3ui.forcePlayerInventoryUpdate()`](https://mwse.github.io/MWSE/apis/tes3ui/#tes3uiforceplayerinventoryupdate) must manually be called after all inventory updates are finished." },
		},
	}},
	returns = {{ name = "removedCount", type = "number" }},
}
