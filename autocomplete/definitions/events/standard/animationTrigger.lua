return {
	type = "event",
	description = "This event is raised when the potential activation target for the player changes.",
	eventData = {
		["reference"] = { type = "tes3reference", readOnly = true, description = "The reference that the animation has been triggered on." },
		["name"] = { type = "string", readOnly = true, description = "The name of the animation triggered." },
		["param"] = { type = "string|nil", readOnly = true, description = "The parameter string associated witht he animation." },
	},
	filter = "name",
}