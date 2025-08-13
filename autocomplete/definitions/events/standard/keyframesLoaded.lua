return {
	type = "event",
	description = "This event is triggered when a keyframes file is loaded.",
	related = { "keyframesLoad" },
	eventData = {
		["path"] = {
			type = "string",
			description = "The path to the keyframes file, relative to Data Files\\Meshes.",
		},
		["keyframe"] = {
			type = "tes3keyframeDefinition",
			description = "The loaded keyframe definition.",
		},
		["sequenceName"] = {
			type = "string",
			description = "The name of the associated NiSequence object.",
		},
		["textKeys"] = {
			type = "niTextKeyExtraData?",
			description = "Convience access to the sequence text keys, or `nil` if it has none.",
		},
	},
	filter = "path",
}
