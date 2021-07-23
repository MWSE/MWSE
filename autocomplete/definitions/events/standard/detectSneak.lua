return {
	description = "This event is invoked whenever an actor is checking if they can detect another actor sneaking.",
	eventData = {
		["detector"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that is doing the detecting.",
		},
		["target"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that is possibly being detected.",
		},
		["isDetected"] = {
			type = "boolean",
			description = "The previous detection calculated, starting with the base engine result.",
		},
	},
}