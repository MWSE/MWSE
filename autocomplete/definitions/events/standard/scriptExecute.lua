return {
	type = "event",
	description = [[This event is fired when a Morrowind script begins executing. It is blockable, so handlers can prevent the engine script call from running.]],
	related = { "scriptExecuted" },
	eventData = {
		["script"] = {
			type = "tes3script",
			readOnly = true,
			description = "The script about to execute.",
		},
		["reference"] = {
			type = "tes3reference|nil",
			readOnly = true,
			description = "The primary reference passed to the script, if any.",
		},
		["reference2"] = {
			type = "tes3reference|nil",
			readOnly = true,
			description = "The secondary reference passed to the script, if any.",
		},
		["variables"] = {
			type = "table",
			readOnly = true,
			description = "A snapshot of the script variables that the execution will use.",
		},
		["info"] = {
			type = "tes3dialogueInfo|nil",
			readOnly = true,
			description = "The dialogue info associated with this script execution, if any.",
		},
	},
	filter = "script",
	blockable = true,
}
