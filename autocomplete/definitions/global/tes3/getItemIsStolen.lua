return {
	type = "function",
	description = [[This function checks item's stolen flag.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3item", description = "The item to check." },
			{ name = "from", type = "tes3creature|tes3npc|tes3faction", optional = true, description = "Where the item was stolen from. If not provided, the function will return true if the item was stolen from anyone." }
		},
	}},
	returns = {
		{ name = "isStolen", type = "boolean", description = "If true the item is stolen." },
		{ name = "stolenFrom", type = "tes3creature[]|tes3npc[]|tes3faction[]", description = "A list of who and what the item has been stolen from." },
	},
}
