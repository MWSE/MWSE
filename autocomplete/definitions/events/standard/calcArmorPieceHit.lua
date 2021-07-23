return {
	description = "This event is raised just after determining which armor piece, if any, was hit with an attack. The slots can be modified, to draw focus onto specific armor slots.",
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile who is being hit.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the mobile's reference.",
		},
		["slot"] = {
			type = "number",
			description = "The primary slot that is targeted.",
		},
		["fallback"] = {
			type = "number",
			description = "A secondary slot to check for equipment on, if no armor is found using the primary slot.",
		},
	},
	filter = "reference.baseObject",
}