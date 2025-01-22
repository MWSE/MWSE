return {
	type = "event",
	description = "This event is triggered when the game is about to play a sound when an item gets added or removed from an inventory, or when eaten or consumed. Can be blocked to play custom sound instead.",
	related = { "soundObjectPlay" },
	eventData = {
		["item"] = {
			type = "tes3baseObject",
			readOnly = true,
			description = "The item for which the sound is about to be played.",
		},
		["state"] = {
			type = "tes3.itemSoundState",
			readOnly = true,
			description = "Maps to values in [tes3.itemSoundState](https://mwse.github.io/MWSE/references/item-sound-states/) namespace.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor reference that picked or dropped the item that's now about to play the sound.",
		},
	},
	filter = "item",
	blockable = true,
}
