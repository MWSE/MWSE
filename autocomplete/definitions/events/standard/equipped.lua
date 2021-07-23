return {
	description = "The equipped event fires after an actor has successfully equipped an item.",
	eventData = {
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
		reference = {
			type = "tes3reference",
			readOnly = true,
			description = "actor’s associated reference.",
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
