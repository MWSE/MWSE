return {
	type = "method",
	description = "Checks to see if the inventory contains an item. This should not be used, instead use the tes3.dropItem function.",
	arguments = {
		{ name = "mobile", type = "tes3mobileActor|tes3reference|string", description = "The mobile actor whose stats will be updated." },
		{ name = "item", type = "tes3item|string", description = "The item to drop." },
		{ name = "itemData", type = "tes3itemData", description = "If provided, it will check for the specific data to drop it." },
		{ name = "count", type = "number", description = "The number of items to drop." },
		{ name = "position", type = "tes3vector3", description = "A vector determining placement location." },
		{ name = "orientation", type = "tes3vector3", description = "A vector determining placement rotation." },
		{ name = "ignoreItemData", type = "boolean" },
	},
}