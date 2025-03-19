return {
	type = "function",
	description = [[Creates itemData on a given `reference`, or `to` a reference's inventory. This can be then used to add custom user data or adjust an item's condition. This will return nil if no item data could be allocated for the item -- for example if the reference doesn't have the item in their inventory or each item of that type already has item data. Calling this function will mark the `reference` or `to` reference as modified.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The reference who will be modified. Use this parameter if you want to add itemData to a reference itself. No other parameters are necessary when this one is used." },
			{ name = "toCursor", type = "boolean", optional = true, description = "Use this parameter if you want to add itemData to an item on the player's cursor. No other parameters are necessary when this one is used." },
			{ name = "to", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The reference or mobile whose inventory will be modified. Use this parameter if you want to add itemData to an item in a reference's inventory." },
			{ name = "item", type = "tes3item|string", optional = true, description = "The item to create item data for. Only applicable if the `to` parameter is used." },
			{ name = "updateGUI", type = "boolean", optional = true, default = true, description = "If false, the player or contents menu won't be updated." },
		},
	}},
	returns = {{ name = "createdData", type = "tes3itemData" }},
}
