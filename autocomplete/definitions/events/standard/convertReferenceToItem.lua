return {
	description = "This event is raised when an in-world tes3reference for an item is about to be converted to fit into a tes3itemStack.",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference about to be converted.",
		},
	},
}