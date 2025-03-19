return {
	type = "function",
	description = [[Loads a toml file from `path`, using `toml.decode`. Unlike the related `json.loadfile` function, this is relative to the current working directory, and not relative to Data Files\MWSE. A file extension should be part of the `path` string.]],
	arguments = {
		{ name = "path", type = "string", description = "The file to read from, relative to the current working directory (typically the Morrowind installation folder)." },
	},
	returns = {
		{ name = "data", type = "table?", description = "The decoded data, or `nil` if the file could not be found." },
		{ name = "error", type = "table?", description = "Information about why the toml file could not be decoded. This result will only be given if the operation fails." },
	},
}
