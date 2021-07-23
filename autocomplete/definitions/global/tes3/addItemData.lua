return {
	type = "function",
	description = [[Creates an item data if there is room for a new stack in a given inventory. This can be then used to add custom user data or adjust an item's condition. This will return nil if no item data could be allocated for the item -- for example if the reference doesn't have the item in their inventory or each item of that type already has item data.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "to", type = "tes3reference|tes3mobileActor|string", description = "The reference or mobile whose inventory will be modified." },
			{ name = "item", type = "tes3item|string", description = "The item to create item data for." },
			{ name = "updateGUI", type = "boolean", default = true, description = "If false, the player or contents menu won't be updated." },
		},
	}},
	returns = {{ name = "createdData", type = "tes3itemData" }},
}
