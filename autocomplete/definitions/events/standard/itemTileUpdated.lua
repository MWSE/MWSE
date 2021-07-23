return {
	description = "This event fires when a dialogue response is triggered.",
	eventData = {
		["element"] = {
			type = "tesuiElement",
			readOnly = true,
			description = "The command.",
		},
		["item"] = {
			type = "tes3item",
			readOnly = true,
			description = "The reference.",
		},
		["itemData"] = {
			type = "tesitemData",
			readOnly = true,
			description = "The script variables.",
		},
		["menu"] = {
			type = "tesuiElement",
			readOnly = true,
			description = "The dialogue object.",
		},
		["tile"] = {
			type = "tes3uiInventoryTile",
			readOnly = true,
			description = "The dialogue info object.",
		},
	},
}