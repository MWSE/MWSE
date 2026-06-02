return {
	type = "event",
	description = [[This event is fired after a Morrowind script finishes executing.]],
	related = { "scriptExecute" },
	eventData = {
		["script"] = {
			type = "tes3script",
			readOnly = true,
			description = "The script that finished executing.",
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
			description = "A snapshot of the script variables that were used during execution.",
		},
		["info"] = {
			type = "tes3dialogueInfo|nil",
			readOnly = true,
			description = "The dialogue info associated with this script execution, if any.",
		},
	},
	filter = "script",
}
