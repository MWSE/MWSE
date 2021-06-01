return {
	description = "The equipped event fires after an actor has successfully equipped an item.",
	eventData = {
		actor = {
			type = "tes3actor",
			readonly = true,
			description = "The actor that has equipped the item.",
		},
		mobile = {
			type = "tes3mobileActor",
			readonly = true,
			description = "The mobile actor that has equipped the item.",
		},
		reference = {
			type = "tes3reference",
			readonly = true,
			description = "actor’s associated reference.",
		},
		item = {
			type = "tes3baseObject",
			readonly = true,
			description = "The object that has been equipped.",
		},
		itemData = {
			type = "tes3itemData",
			readonly = true,
			description = "The item data of item.",
		},
	},
    	filter = "item",
	links = {
        ["equip"] = "lua/event/equip",
    },
}
