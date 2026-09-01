return {
	type = "method",
	description = [[Updates a simple playing sound. Does not load or autoplay a sound if it is already loaded.]],
	arguments = {
		{ name = "sound", type = "tes3sound", description = "The sound to play." },
		{ name = "volume", type = "number", description = "The scalar for the volume. The final volume played will use the master and effect volume scalars." },
	},
}
