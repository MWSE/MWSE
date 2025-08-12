return {
	type = "event",
	description = [[This event is called when an animation is triggered using the LuaEvent key.]],
	eventData = {
		["reference"] = { type = "tes3reference", readOnly = true, description = "The reference that the animation has been triggered on." },
		["name"] = { type = "string", readOnly = true, description = "The name of the animation triggered." },
		["param"] = { type = "string|nil", readOnly = true, description = "The parameter string associated witht he animation." },
	},
	filter = "name",
}