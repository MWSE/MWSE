return {
	type = "event",
	description = "This event fires whenever a tes3dialogueInfo has been selected for a given tes3dialogue. This includes but is not limited to showing greetings, finding voice overs, and calculating service requirements.",
	related = { "dialogueFiltered", "infoGetText", "infoLinkResolve", "infoResponse", "infoFilter", "postInfoResponse" },
	eventData = {
		["dialogue"] = {
			type = "tes3dialogue",
			readOnly = true,
			description = "The dialogue that was filtered.",
		},
		["info"] = {
			type = "tes3dialogueInfo",
			readOnly = true,
			description = "The result that the filter returned.",
		},
		["actor"] = {
			type = "tes3actor",
			readOnly = true,
			description = "Any associated actor for the query.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Any associated reference for the query.",
		},
		["context"] = {
			type = "tes3.dialogueFilterContext",
			readOnly = true,
			description = "What caused the dialogue filtering. Maps to values in the [`tes3.dialogueFilterContext`](https://mwse.github.io/MWSE/references/dialogue-filter-context/) table.",
		},
	},
}
