return {
	type = "event",
	description = "This event is triggered after an NPC's disposition is calculated. This is done frequently. The disposition value can be changed to alter the resulting value. The disposition should be consistent between calls.",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference whose disposition is being calculated.",
		},
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "Convenient access to the `reference`'s mobile. The mobile may not be available if the target has not been visited recently, e.g. when scripts access dispositions.",
		},
		["disposition"] = {
			type = "number",
			description = "The precalculated disposition. This can be set to override the value.",
		},
		["clamped"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, the `disposition` value will end up clamped between 0 and 100.",
		},
	},
	filter = "reference",
	examples = {
		["Vanilla"] = {
			title = "Reimplementation of the vanilla disposition calculation.",
		},
	},
}
