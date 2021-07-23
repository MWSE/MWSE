return {
	type = "method",
	description = [[Removes an item from the inventory directly. This should not be used, in favor of the tes3.removeItem function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "mobile", type = "tes3mobileActor|tes3reference|string", optional = true, description = "The mobile actor whose stats will be updated." },
			{ name = "item", type = "tes3item", description = "The item to add." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "Any associated item data to add." },
			{ name = "count", type = "number", default = 1, description = "The number of items to add." },
			{ name = "deleteItemData", type = "boolean", default = false, description = "If set, the itemData will be deleted after being removed." },
		}
	}},
}