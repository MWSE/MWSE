return {
	type = "function",
	description = [[Fetches the contents of the [metadata file](https://mwse.github.io/MWSE/guides/metadata/) associated with a given lua mod key.
The mod key should match the value of `lua-mod` specified in the `[tools.mwse]` section of the relevant metadata file.]],
	arguments = {
		{ name = "modKey", type = "string", description = "The key for the lua mod, which must match the file location and the metadata file's `[tools.mwse]` contents." },
	},
	returns = {
		{ name = "metadata", type = "MWSE.Metadata|nil" },
	}
}
