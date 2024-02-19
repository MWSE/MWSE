return {
	type = "function",
	description = [[Loads the contents of a file through `json.decode`. Files loaded from "Data Files\\MWSE\\{`fileName`}.json".

!!! warning "json does not support `number` indices"
	The `table` returned by this function won't have any `number` indices. (e.g., the first key of an array-style table will be decoded as `["1"]`, not as `[1`].)
	You should be mindful of this when using this function.
	If you're using this to load a configuration file for your mod, it's recommended you use [`mwse.loadConfig`](https://mwse.github.io/MWSE/apis/mwse/#mwseloadconfig) instead.
]],
	arguments = {
		{ name = "fileName", type = "string" },
	},
	valuetype = "table",
}