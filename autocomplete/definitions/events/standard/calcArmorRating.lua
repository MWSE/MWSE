return {
	description = "This event is fired before an actor's armor rating has been calculated, and can be used to override the armor that the actor is given.",
	eventData = {
		["armor"] = {
			type = "tes3armor",
			readOnly = true,
			description = "The armor piece whose AR is being calculated.",
		},
		["armorRating"] = {
			type = "number",
			description = "If set, the given armor rating will be used instead of the one calculated.",
		},
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile whose AR is being calculated. May not always be available.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the mobile's reference. May not always be available.",
		},
		["npc"] = {
			type = "tes3npc",
			readOnly = true,
			description = "The NPC object whose AR is being calculated. May not always be available.",
		},
	},
	filter = "armor",
}