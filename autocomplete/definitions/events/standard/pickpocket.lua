return {
	type = "event",
	description = "This event fires when the player attempts to pickpocket an item stack, and also on closing the pickpocket contents window. When the window is closed, `item` will be `nil`.",
	related = { "trapDisarm" },
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor being pickpocketed.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference of the actor being pickpocketed.",
		},
		["item"] = {
			type = "tes3item|nil",
			readOnly = true,
			description = "The chosen item. `nil` when the window is being closed.",
		},
		["itemData"] = {
			type = "tes3itemData|nil",
			readOnly = true,
			description = "The chosen item data. `nil` when the window is being closed.",
		},
		["count"] = {
			type = "number",
			readOnly = true,
			description = "The count of items in the chosen stack.",
		},
		["chance"] = {
			type = "number",
			description = "The % chance the pickpocket attempt will be successful.",
		},
	},
}