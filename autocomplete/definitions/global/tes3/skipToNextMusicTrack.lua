return {
	type = "function",
	description = [[This function interrupts the current music to play a random new combat or explore track, as appropriate. The selected music track can be read from the audio controller's `.nextMusicFilePath` field.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "situation", type = "tes3.musicSituation", optional = true, description = [[Determines what kind of gameplay situation the music should activate for. By default, the function will determine the right solution based on the player's combat state. This value maps to [`tes3.musicSituation`](https://mwse.github.io/MWSE/references/music-situations/) constants.]] },
			{ name = "crossfade", type = "number", optional = true, default = "1.0", description = "The duration in seconds of the crossfade from the old to the new track. The default is 1.0." },
			{ name = "volume", type = "number", optional = true, description = "The volume at which the music will play. If no volume is provided, the user's volume setting will be used." },
			{ name = "force", type = "boolean", default = "false", description = "If true, normally uninterruptible music will be overwritten to instead play the new track." },
		},
	}},
	returns = {{ name = "musicTrackQueued", type = "boolean" }},
}
