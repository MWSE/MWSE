return {
	type = "method",
	description = [[This metod plays the sound.]],
	arguments = {
		{ name = "playbackFlags", type = "number", optional = true, description = "These match low-level DirectSound flags. Most mods don't need to modify this, though a value of 1 to loop may be useful. The rest of the flags can be read in the IDirectSoundBuffer8::Play documentation: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee418074(v=vs.85)." },
		{ name = "volume", type = "number", optional = true },
        	{ name = "pitch", type = "number", optional = true },
        	{ name = "isNot3D", type = "boolean", optional = true },
	},
}
