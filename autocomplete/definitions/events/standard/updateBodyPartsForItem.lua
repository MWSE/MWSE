return {
	type = "event",
	description = "This event is called prior to adding body parts to for a piece of equipment (armor or clothing).",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Whose body parts are being built for the given item.",
		},
		["bodyPartManager"] = {
			type = "tes3bodyPartManager",
			readOnly = true,
			description = "`reference`'s body part manager.",
		},
		["item"] = {
			type = "tes3item",
			description = "The item whose parts will be used. This can be changed to provide an override. Note that items must be of the same type (i.e. armor to armor, but not armor to clothing).",
		},
		["isFemale"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, the reference is female.",
		},
		["isFirstPerson"] = {
			type = "boolean",
			readOnly = true,
			description = "If true, the reference is first person.",
		},
	},
	filter = "item",
	blockable = true,
}