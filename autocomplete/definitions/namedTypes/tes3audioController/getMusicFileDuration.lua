return {
	type = "method",
	description = [[Loads a music file and determines its duration, in seconds.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to the music track, relative to the game installation directory." },
	},
	returns = {
		{ name = "duration", type = "number", description = "The number of seconds of the track, in seconds." },
	},
}
