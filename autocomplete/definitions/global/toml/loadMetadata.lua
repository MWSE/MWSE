return {
	type = "function",
	description = [[Loads a toml metadata file with a given key. This function is identical to `toml.loadFile` with a mod's metadata key to determine its path. Active lua mods already have their metadata loaded, which can be retrieved using `tes3.getLuaModMetadata()`.]],
	arguments = {
		{ name = "key", type = "string", description = "The key for the metadata. This is the prefix before `-metadata.toml`, and matches a file found in Data Files." },
	},
	returns = {
		{ name = "data", type = "table?", description = "The decoded data, or `nil` if the file could not be loaded." },
	},
}
