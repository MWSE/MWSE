return {
	type = "function",
	description = [[Loads the contents of a file through json.decode. Files loaded from Data Files\\MWSE\\{fileName}.json.

!!! warning
	All of the keys in the `table` returned by function will be `string`s. For example, if you're decoding an array-style table, then the first entry will be `["1"]` instead of `[1]`.
	If you're using this function to load your mod's configuration file, consider using [`mwse.loadConfig()`](https://mwse.github.io/MWSE/apis/mwse/#mwseloadconfig) instead.
]],
	arguments = {
		{ name = "fileName", type = "string" },
	},
	valuetype = "table",
}