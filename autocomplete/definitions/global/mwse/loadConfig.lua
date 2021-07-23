return {
	type = "function",
	description = [[Loads a config table from Data Files\MWSE\config\{fileName}.json.
	
If the default values table is passed, empty keys in the config will be filled in using its values. Additionally, if no file exists, the function will return the default table.]],
	arguments = {
		{ name = "fileName", type = "string", description = "The non-extensioned name of the config file." },
		{ name = "defaults", type = "table", optional = true, description = "A table of default values." },
	},
	returns = "table",
}