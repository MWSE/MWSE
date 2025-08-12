return {
	type = "function",
	description = [[Loads a config table from Data Files\\MWSE\\config\\{fileName}.json.

If the default values table is passed:

 - Empty keys in the config will be filled in using its values.
 - If no file exists, the function will return the default table.
 - In json, tables can be either arrays with integer keys or dictionaries with string keys. If your configuration table is mixed (has both string and integer indices), saving and loading from json will effectively convert all your integer indices to strings. This function will convert your configuration table's integer indices back if defaults table is given.
]],
	generics = {
		{ name = "configType", inherits = "table" }
	},
	arguments = {
		{ name = "fileName", type = "string", description = "The non-extensioned name of the config file." },
		{ name = "defaults", type = "configType", optional = true, description = "A table of default values." },
	},
	returns = "configType",
}