return {
	type = "event",
	description = "This event is called when one object's data is copied to another object.",
	eventData = {
		["copy"] = {
			type = "tes3object",
			readOnly = true,
			description = "The object being copied to.",
		},
		["original"] = {
			type = "tes3object",
			readOnly = true,
			description = "The object being copied from.",
		},
	},
	filter = "original",
}
