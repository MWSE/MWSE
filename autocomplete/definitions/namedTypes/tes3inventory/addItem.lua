return {
	type = "method",
	description = [[Adds an item into the inventory directly. This should not be used, in favor of the [`tes3.addItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3additem) function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "mobile", type = "tes3mobileActor|tes3reference|string", optional = true, description = "The mobile actor whose stats will be updated." },
			{ name = "item", type = "tes3item|tes3leveledItem|string", description = "The item to add. Leveled items will only work properly when added to inventories of non-instanced objects." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "Any associated item data to add." },
			{ name = "count", type = "number", optional = true, default = 1, description = "The number of items to add." },
		}
	}},
}