return {
	type = "function",
	description = [[Pays a merchant gold. The money is transferred to their barter gold (non-inventory trading gold), and also updates the last barter timer, so that it works the same way a transaction affeects the barter gold reset cycle. This is useful for simulating paying for services. The function will return true if there was enough gold to complete the payment.

A negative cost will allow payment from the merchant's barter gold to the player. You may also want to play a trade-related sound of your choice upon successful completion.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "merchant", type = "tes3mobileActor", description = "The merchant to pay." },
			{ name = "cost", type = "number", description = "The amount of gold to transfer to the merchant. May be negative to transfer gold to the player." },
		},
	}},
	returns = {
		{ name = "success", type = "boolean", description = "True if the transaction completed. False if there was not enough gold." },
	},
}
