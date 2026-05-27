return {
	type = "event",
	description = "This event is raised when reference is invalidated. This includes being removed from memory. This event can be used to safely remove references from tables.",
	eventData = {
		["object"] = {
			type = "tes3baseObject|tes3mobileObject|tes3uiElement|niObject",
			readOnly = true,
			description = "The object being invalidated. This is a stale pointer late in its deconstruction, and should not be used to do any logic or rely on any internal information.",
		},
	},
}