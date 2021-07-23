return {
	description = "This event is called when determining the price of repairing an item. It can be used to override the repair cost.",
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
			description = "The adjusted price of repairing item.",
		},
		["item"] = {
			type = "tes3item",
			readOnly = true,
			description = "The item to be repaired.",
		},
		["itemData"] = {
			type = "tes3item",
			readOnly = true,
			description = "Variable node. The data associated with item. This may be nil.",
		},
	},
}