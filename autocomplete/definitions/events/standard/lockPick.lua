return {
	description = "This event fires when a lock is being picked.",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference that triggered the event (container, door, etc.).",
		},
		["lockData"] = {
			type = "tes3lockAttachmentNode",
			readOnly = true,
			description = "The lock data of the reference.",
		},
		["picker"] = {
			type = "tes3mobileNpc",
			readOnly = true,
			description = "The Mobile NPC doing the disarming.",
		},
		["tool"] = {
			type = "tes3item",
			readOnly = true,
			description = "The item the picker is using to pick the lock.",
		},
		["toolItemData"] = {
			type = "tes3itemData",
			readOnly = true,
			description = "The item data for the tool.",
		},
		["chance"] = {
			type = "number",
			readOnly = true,
			description = "The chance the lockpick will be successful.",
		},
		["lockPresent"] = {
			type = "bool",
			readOnly = true,
			description = "Indicates if a lock is present on the reference.",
		},
	}
}