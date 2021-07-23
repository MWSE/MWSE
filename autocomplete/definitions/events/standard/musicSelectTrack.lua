return {
	description = "The musicSelectTrack event occurs when new music is needed after a playing music track ends, or the combat situation changes. It allows you to select your own music for the current conditions by setting eventData.music. However, it does not control transitions to combat music, which in the future will be available in another event.",
	eventData = {
		["situation"] = {
			type = "number",
			readOnly = true,
			description = "Maps to tes3.musicSituation.*, indicating combat or non-combat music.",
		},
		["music"] = {
			type = "string",
			optional = true,
			description = "If set to the path of a given track (relative to Data Files/music), it will play the given path instead of a random one.",
		},
	},
}