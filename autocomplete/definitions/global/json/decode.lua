return {
	type = "function",
	description = [[Decode string into a table.

!!! warning "json does not support mixed `string` and `number` indices"
	If the encoded table had any `string` indices, then the `table` returned by this function will have no `number` indices. For example, `[1]` could have been converted to `["1"]` in the encoding process.
]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "s", type = "string" },
		{ name = "position", type = "number", optional = true, default = 1 },
		{ name = "nullValue", type = "string", optional = true },
	},
	valuetype = "table",
}