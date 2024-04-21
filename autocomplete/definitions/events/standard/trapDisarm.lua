return {
	type = "event",
	description = "This event fires when a trap is being disarmed.",
	related = { "lockPick" },
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference that triggered the event (container, door, etc.).",
		},
		["lockData"] = {
			type = "tes3lockNode",
			readOnly = true,
			description = "The lock data of the reference.",
		},
		["disarmer"] = {
			type = "tes3mobileNPC",
			readOnly = true,
			description = "The Mobile NPC doing the disarming.",
		},
		["tool"] = {
			type = "tes3item",
			readOnly = true,
			description = "The item the disarmer is using to disarm the trap.",
		},
		["toolItemData"] = {
			type = "tes3itemData",
			readOnly = true,
			description = "The item data for the tool.",
		},
		["chance"] = {
			type = "number",
			description = "The chance that the probe attempt will be successful. May be modified. If set to a value `<= 0`, the attempt will fail and the \"Trap too complex\" message will be displayed.",
		},
		["trapPresent"] = {
			type = "boolean",
			readOnly = true,
			description = "Indicates if a trap is present on the reference.",
		},
	},
	blockable = true,
}