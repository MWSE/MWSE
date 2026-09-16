return {
	type = "method",
	description = [[Updates the volume of a looping sound. Also handles loading the sound if it isn't given, and will return the loaded sound asnd if it is played. Additionally, it handles underwater frequency changes.]],
	arguments = {
		{ name = "sound", type = "tes3sound", description = "The sound to play. If `nil`, the sound will be loaded using the `soundId` ID and returned as the first value." },
		{ name = "soundId", type = "string", description = "The ID of the sound to play. Used to load the sound, if `sound` is `nil`." },
		{ name = "volume", type = "number", description = "The scalar for the volume. The final volume played will use the master and effect volume scalars." },
		{ name = "shouldPlay", type = "boolean", description = "If true, the sound will continue playing, update its volume, and track underwater frequency changes. If false, the sound will stop playing. This function's second returned value will be true if the sound exists and should play." },
	},
	returns = {
		{ name = "sound", type = "tes3sound|nil", description = "The sound given, the newly located/loaded sound, or nil if the sound could not be resolved." },
		{ name = "playing", type = "boolean", description = "True if the sound is playing, or false if it is not." },
	},
}
