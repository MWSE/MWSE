return {
	type = "event",
	description = "This event is called when a new object is created.",
	eventData = {
		["object"] = {
			type = "tes3baseObject",
			readOnly = true,
			description = "The object that was created.",
		},
		["copiedFrom"] = {
			type = "tes3object",
			readOnly = true,
			description = "If the object was created by first copying another object, that object is available here.",
		},
	},
	filter = "object",
}
