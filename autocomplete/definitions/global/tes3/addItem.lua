return {
	type = "function",
	description = [[Adds an item to a given reference's inventory or mobile's inventory. The `reference` will be cloned if needed.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "Who to give items to." },
			{ name = "item", type = "tes3item|tes3leveledItem|string", description = "The item to add. If a leveled item is passed, it will be resolved and added." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data for the item. The owner, if set, will be cleared. Note that this may be deleted from memory then ignored if it has no other special information associated with it (i.e., it is fully repaired/charged, has no soul, and contains empty lua data)." },
			{ name = "soul", type = "tes3creature|tes3npc", optional = true, description = "For creating filled soul gems." },
			{ name = "count", type = "number", optional = true, default = "1", description = "The maximum number of items to add." },
			{ name = "playSound", type = "boolean", optional = true, default = true, description = "If `false`, the up/down sound for the item won't be played. This only applies if `reference` is the player." },
			{ name = "showMessage", type = "boolean", optional = true, default = false, description = "If `true`, a message box notifying the player will be shown. This only applies if `reference` is the player." },
			{ name = "limit", type = "boolean", optional = true, default = false, description = "If `false`, items can be placed into containers that shouldn't normally be allowed. This includes organic containers, and containers that are full." },
			{ name = "reevaluateEquipment", type = "boolean", optional = true, default = true, description = "If `true`, and the item added is armor, clothing, or a weapon, the actor will reevaluate its equipment choices to see if the new item is worth equipping. This does not affect the player." },
			{ name = "equipProjectiles", type = "boolean", optional = true, default = true, description = "If `true`, and the reference has the same projectile already equipped, the stacks will be merged. This will only work if the GUI is updated." },
			{ name = "updateGUI", type = "boolean", optional = true, default = true, description = "If `false`, the function won't manually resync the player's GUI state. This can result in some optimizations, though [`tes3ui.forcePlayerInventoryUpdate()`](https://mwse.github.io/MWSE/apis/tes3ui/#tes3uiforceplayerinventoryupdate) must manually be called after all inventory updates are finished." },
		},
	}},
	returns = {
		{ name = "count", type = "number", description = "The number of items added to the reference." },
		{ name = "item", type = "tes3item", description = "The item added. This is usually the `item` parameter passed to the function, but can be something else in the case of leveled items." },
		{ name = "itemData", type = "tes3itemData", description = "The itemData added. This can be created if the `soul` parameter is used, or if an `itemData` was passed. If the passed `itemData` was deleted, the value will be `nil`." },
	},
}
