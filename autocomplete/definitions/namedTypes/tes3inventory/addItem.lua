return {
	type = "method",
	description = [[Adds an item into the inventory directly. This should not be used, in favor of the [`tes3.addItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3additem) function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "mobile", type = "tes3mobileActor|tes3reference|string", optional = true, description = "The mobile actor whose stats will be updated." },
			{ name = "item", type = "tes3item|tes3leveledItem", description = "The item or leveled item to add. If adding a leveled item to an inventory of a cloned object (such as [tes3containerInstance](../types/tes3containerInstance.md)), the leveled list will be resolved. Otherwise the leveled item record is added to the inventory directly." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "Any associated item data to add." },
			{ name = "count", type = "number", optional = true, default = 1, description = "The number of items to add." },
		}
	}},
}