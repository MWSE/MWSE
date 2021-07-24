return {
	description = [[The equip event fires when an actor is about to equip an item, i.e. just before the item is equipped. This event allows scripts to block equipping. See `equipped`_ for the post-equip event.

The item must not be removed from the inventory during this event, as the item and item data is still being referenced by the initiator of the equip action. If you want to consume the item, block the equip and remove it at a later time. Removing the item during this event can cause inventory desync bugs.]],
	related = { "equip", "equipped", "unequipped" },
	eventData = {
		reference = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference to the mobile actor that is equipping the item.",
		},
		item = {
			type = "tes3baseObject",
			readOnly = true,
			description = "The object being equipped.",
		},
		itemData = {
			type = "tes3itemData",
			readOnly = true,
			description = "The item data of item.",
		},
	},
	filter = "item",
	links = {
		["equipped"] = "lua/event/equipped",
	},
}
