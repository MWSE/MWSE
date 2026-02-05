return {
	type = "event",
	description = "This event is triggered before a keyframes file is loaded. The path can be changed to instead load a different keyframes file.",
	related = { "meshLoad", "keyframesLoaded", "keyframesParsed" },
	eventData = {
		["path"] = {
			type = "string",
			description = "The path to the keyframes file, relative to Data Files\\Meshes.",
		},
		["sequenceName"] = {
			type = "string",
			description = "The name of the associated NiSequence object.",
		},
	},
	filter = "path",
}
