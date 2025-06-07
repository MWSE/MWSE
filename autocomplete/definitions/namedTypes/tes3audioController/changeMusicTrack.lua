return {
	type = "method",
	description = [[Changes currently playing music track according to provided parameters.]],
	arguments = {
		{ name = "filename", type = "string", description = "File name of the new music track." },
		{ name = "crossfade", type = "number", optional = true, default = 1000, description = "Duration of the crossfading between music tracks. Measured in miliseconds." },
		{ name = "volume", type = "number", optional = true, default = 1.0, description = "Allows changing the volume of the music track." },
	}
}
