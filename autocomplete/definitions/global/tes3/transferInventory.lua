return {
	type = "function",
	description = [[Moves all the items in one reference's inventory to another. Both `to` and `from` objects will be cloned. The function will update the GUI for the `to` and `from` references. This function preserves the `tes3itemData` of the transferred items and handles leveled lists. The function can do either partial or complete transfer. Limiting transfer by capacity only works for containers, other actors can get over-encumbered after this operation.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "from", type = "tes3reference|tes3mobileActor|string", description = "Who to take items from." },
			{ name = "to", type = "tes3reference|tes3mobileActor|string", description = "Who to give items to." },
			{ name = "filter", type = "fun(item: tes3item, itemData?: tes3itemData): boolean", optional = true, description = "You can pass a filter function to only transfer certain type of items. The `filter` function is called for each item in the `from`'s inventory. Note that not all the items may have itemData." },
			{ name = "playSound", type = "boolean", optional = true, default = true, description = "If false, the up/down sound won't be played." },
			{ name = "limitCapacity", type = "boolean", optional = true, default = true, description = "If false, items can be placed into containers that shouldn't normally be allowed. This includes organic containers and containers that are full. If this argument is set to `true` the whole `from`'s inventory might not fit into the destination inventory. In that case, partial transfer is made." },
			{ name = "completeTransfer", type = "boolean", optional = true, default = false, description = "Use this to disable partial transfers. If `limitCapacity` is set to true, passing `completeTransfer = true` will only transfer the items from one inventory to the other if and only if all the items can fit inside the destination inventory. This argument only works if `limitCapacity` is `true`." },
			{ name = "reevaluateEquipment", type = "boolean", optional = true, default = true, description = "If true, and the if in the transferred items are armor, clothing, or weapon items, the actors will reevaluate their equipment choices to see if the new items are worth equipping. This does not affect the player." },
			{ name = "equipProjectiles", type = "boolean", optional = true, default = true, description = "If true, and the `to` reference has the same projectile already equipped, the stacks will be merged." },
			{ name = "checkCrime", type = "boolean", optional = true, default = false, description = "If true, and the `to` reference is the player, the function will check if the player has access to the `from` reference's inventory. If not, appropriate crime reactions will be triggered." },
		},
	}},
	returns = {{ name = "transferred", type = "boolean", description = "Returns `true` if at least one item was transferred. If both `limitCapacity` and `completeTransfer` were passed as `true` the function returns `true` if the whole inventory was successfully transferred." }},
	examples = {
		["filter"] = {
			title = "Transfering target's weapons to the player's inventory"
		}
	}
}
