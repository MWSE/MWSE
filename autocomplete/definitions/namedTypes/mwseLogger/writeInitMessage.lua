return {
	type = "method",
	description = [[Writes an `INFO` message saying this mod has been initialized. 
If your mod has [metadata file](../guides/metadata.md#package-section) that specifies its current version,
then that will also be included in the initialization message. 
You may also supply a version number directly as an argument to this method.]],
	arguments = {
		{ name = "version", type = "string", optional = true, description = "The current version of your mod. If not provided, the logger will attempt to retrieve it from your mod's metadata file." },
	}
}