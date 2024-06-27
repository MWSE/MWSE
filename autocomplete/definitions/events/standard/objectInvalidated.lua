return {
	type = "event",
	description = "This event is raised when reference is invalidated. This includes being removed from memory. This event can be used to safely remove references from tables.",
	eventData = {
		["object"] = {
			type = "tes3baseObject",
			readOnly = true,
			description = "The object being invalidated. Can be a tes3reference.",
		},
	},
}