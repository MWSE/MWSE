return {
	type = "function",
	description = [[Loads the contents of a file through `json.decode`. Files loaded from "Data Files\\MWSE\\{`fileName`}.json".

!!! warning "json does not support mixed `string` and `number` indices"
	If the encoded table had any `string` indices, then the `table` returned by this function will have no `number` indices. For example, `[1]` could have been converted to `["1"]` in the encoding process.
	If you're using this to load a configuration file for your mod, it's recommended you use [`mwse.loadConfig`](https://mwse.github.io/MWSE/apis/mwse/#mwseloadconfig) instead.
]],
	arguments = {
		{ name = "fileName", type = "string" },
	},
	valuetype = "table",
}