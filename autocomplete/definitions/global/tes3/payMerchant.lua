return {
	type = "function",
	description = [[Pays a merchant a specified amount of gold and updates the merchant's "last barter timer". This should be used to simulate paying for services. You may also want to play a trade-related sound of your choice upon successful completion.

If `cost` is positive, then that amount of gold will be removed from the player's inventory and added to the merchant's available barter gold.

If `cost` is negative, then that amount of gold will be added to the player's inventory and removed from the merchant's available barter gold.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "merchant", type = "tes3mobileActor", description = "The merchant to pay." },
			{ name = "cost", type = "number", description = "The amount of gold to pay the merchant. If negative, the merchant will pay the player." },
		},
	}},
	returns = {
		{ name = "success", type = "boolean", description = "`true` if the transaction completed. `false` if there was not enough gold." },
	},
}
