return {
	description = "This event is invoked when a rest is about to be interrupted. The ambushing spawn can be overridden by changing the creature parameter.",
	eventData = {
		["creature"] = {
			type = "tes3creature|tes3npc|tes3leveledCreature",
			description = "The creature that is interrupting rest. Can be set to a creature, NPC, or leveled creature.",
		},
		["resting"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, the player is attempting to rest, as opposed to wait.",
		},
		["waiting"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, the player is attempting to wait, as opposed to rest.",
		},
	},
}