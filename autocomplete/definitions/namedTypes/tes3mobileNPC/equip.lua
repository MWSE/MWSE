return {
	type = "method",
	description = [[Equips an item, optionally adding the item if needed. If the best match is already equipped, it does not perform an unequip-equip cycle, but does return true.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3item|string", description = "The item to equip." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data of the specific item to equip." },
			{ name = "addItem", type = "boolean", default = false, description = "If true, the item will be added to the actor's inventory if needed." },
			{ name = "selectBestCondition", type = "boolean", default = false, description = "If true, the item in the inventory with the best condition and best charge will be selected." },
			{ name = "selectWorstCondition", type = "boolean", default = false, description = "If true, the item in the inventory with the worst condition and worst charge will be selected. Can be useful for selecting tools." },
		}
	}},
	returns = "itemEquipped",
	valuetype = "boolean",
}