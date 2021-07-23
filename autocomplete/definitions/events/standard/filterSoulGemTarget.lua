return {
	description = "This event fires when a soul gem target is filtered",
	eventData = {
		["soulGem"] = {
			type = "tes3misc",
			readOnly = true,
			description = "The soul gem object.",
		},
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference.",
		},
	},
}