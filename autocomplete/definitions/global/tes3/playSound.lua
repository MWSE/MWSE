return {
	type = "function",
	description = [[Plays a sound on a given reference. Provides control over volume (including volume channel), pitch, and loop control.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "sound", type = "tes3sound|string", description = "The sound object, or id of the sound to look for." },
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "The reference to attach the sound to.", optional = true },
			{ name = "loop", type = "boolean", default = false, description = "If true, the sound will loop." },
			{ name = "mixChannel", type = "number", default = "tes3.audioMixType.effects", description = "The channel to base volume off of. Maps to tes3.audioMixType constants." },
			{ name = "volume", type = "number", default = "1.0", description = "A value between 0.0 and 1.0 to scale the volume off of." },
			{ name = "pitch", type = "number", default = "1.0", description = "The pitch-shift multiplier. For 22kHz audio (most typical) it can have the range [0.005, 4.5]; for 44kHz audio it can have the range [0.0025, 2.25]." },
			{ name = "soundPath", type = "string", description = "The path to a custom soundfile (useful for playing sounds that are not registered in the Construction Set). Starts in Data Files\\Sound.", optional = true },
		},
	}},
	returns = {{ name = "executed", type = "boolean" }},
}
