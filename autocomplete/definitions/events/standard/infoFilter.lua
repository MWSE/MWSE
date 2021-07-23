return {
	description = "This event fires when a dialogue info object is filtered.",
	eventData = {
		["info"] = {
			type = "tes3dialogueInfo",
			readOnly = true,
			description = "The dialogue info object.",
		},
		["actor"] = {
			type = "tes3object",
			readOnly = true,
			description = "The actor.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference.",
		},
		["source"] = {
			type = "number",
			readOnly = true,
			description = "The source.",
		},
		["dialogue"] = {
			type = "tes3dialogue",
			readOnly = true,
			description = "The dialogue object.",
		},
		["passes"] = {
			type = "bool",
			readOnly = true,
			description = "Indicates if the filter passes.",
		},
	},
}