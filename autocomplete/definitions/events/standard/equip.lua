return {
	description = "The equip event fires when an actor is about to equip an item. This event allows scripts to block equipping.",
	eventData = {
		reference = {
			type = "tes3reference",
			readonly = true,
			description = "A reference to the mobile actor that is equipping the item.",
		},
		item = {
			type = "tes3baseObject",
			readonly = true,
			description = "The object being equipped.",
		},
		itemData = {
			type = "tes3itemData",
			readonly = true,
			description = "The item data of item.",
		},
	},
    	filter = "item",
}
