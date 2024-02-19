return {
	type = "function",
	description = [[Decode string into a table.

!!! warning "json does not support `integer` indices"
	As a result, the `table` returned by this function won't have any integer indices. (e.g., the first key of an array-style table will be decoded as `["1"]`, not as `[1`].)
	You should be mindful of this when using this function.
]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "s", type = "string" },
		{ name = "position", type = "number", optional = true, default = 1 },
		{ name = "nullValue", type = "string|nil", optional = true, default = "nil" },
	},
	valuetype = "table",
}