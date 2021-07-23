return {
	description = "This event is called when determining the cost of purchasing a spell. The price can be modified.",
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor of the merchant the player is interacting with.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "mobile’s related reference.",
		},
		["basePrice"] = {
			type = "number",
			readOnly = true,
			description = "The price before adjustment.",
		},
		["price"] = {
			type = "number",
			description = "The adjusted price of spell.",
		},
		["spell"] = {
			type = "tes3spell",
			readOnly = true,
			description = "The spell currently being purchased.",
		},
	},
}