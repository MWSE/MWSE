return {
	description = "This event fires immediately after a dialogue response is processed.",
	eventData = {
		["command"] = {
			type = "string",
			readOnly = true,
			description = "The command.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference.",
		},
		["variables"] = {
			type = "tes3scriptVariables",
			readOnly = true,
			description = "The script variables.",
		},
		["dialogue"] = {
			type = "tes3dialogue",
			readOnly = true,
			description = "The dialogue object.",
		},
		["info"] = {
			type = "tes3dialogueInfo",
			readOnly = true,
			description = "The dialogue info object.",
		},
	},
}