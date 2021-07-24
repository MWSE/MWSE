return {
	description = "The equipped event fires after an actor has successfully equipped an item.",
	related = { "equip", "equipped", "unequipped" },
	eventData = {
		reference = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference to the mobile actor that has equipped the item.",
		},
		actor = {
			type = "tes3actor",
			readOnly = true,
			description = "The actor that has equipped the item.",
		},
		mobile = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that has equipped the item.",
		},
		item = {
			type = "tes3baseObject",
			readOnly = true,
			description = "The object that has been equipped.",
		},
		itemData = {
			type = "tes3itemData",
			readOnly = true,
			description = "The item data of item.",
		},
	},
	filter = "item",
	links = {
		["equip"] = "lua/event/equip",
	},
}
