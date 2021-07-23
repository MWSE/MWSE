return {
	description = "This event fires when an inventory filter has been selected for an item.",
	eventData = {
		["item"] = {
			type = "tes3item",
			readOnly = true,
			description = "The item being filtered.",
		},
		["itemData"] = {
			type = "tes3itemData",
			readOnly = true,
			description = "The item data for the item being filtered.",
		},
		["type"] = {
			type = "string",
			readOnly = true,
			description = "The inventory filter type.",
		},
	},
}