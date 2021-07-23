return {
	type = "function",
	description = [[Drops one or more items from a reference's inventory onto the ground at their feet. It will unequip the item if it is equipped. The return value will be nil if no matching item was found.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3mobileActor|tes3reference|string", description = "The reference whose inventory will be modified." },
			{ name = "item", type = "tes3item|string", description = "The item to drop." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data of the specific item to drop. Without this, the first matching item in the inventory will drop." },
			{ name = "matchNoItemData", type = "boolean", default = false, description = "If true, matches an item without item data. This can be used when you want to drop an item that isn't equipped (equipped items always have item data)." },
			{ name = "count", type = "number", default = 1, description = "The number of items to drop." },
			{ name = "updateGUI", type = "boolean", default = true, description = "If false, the player or contents menu won't be updated." },
		},
	}},
	returns = {{ name = "createdReference", type = "tes3reference" }},
}
