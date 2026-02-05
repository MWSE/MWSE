return {
	type = "event",
	description = "This event is triggered when keyframes have been parsed, but before they have been loaded into animation groups. It can be used to add, remove, or convert file keyframe data.",
	related = { "meshLoad", "keyframesLoad", "keyframesLoaded" },
	eventData = {
		["path"] = {
			type = "string",
			description = "The path to the keyframes file, relative to Data Files\\Meshes.",
		},
		["textKeys"] = {
			type = "{time: number, key: string, value: string}[]",
			description = "An array of the loaded text key definition. Entries can be modified or inserted as tables with the keys `time`, `key`, and `value`.",
		},
	},
	filter = "path",
}
