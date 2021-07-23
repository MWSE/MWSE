return {
	description = "This event triggers when an actor dies.",
	eventData = {
		mobile = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that has died.",
		},
		reference = {
			type = "tes3reference",
			readOnly = true,
			description = "mobile’s associated reference.",
		},
	},
}