return {
	type = "function",
	description = [[Create a string representing the object. Object can be a `table`, `string`, `number`, `boolean`, `nil`, `json.null`,  or any object with a `__tojson` function in its `metatable`. A `table` can only use strings and numbers as keys, and its values have to be valid objects as well. This function will raise an error if called on an invalid data type or on a data struture that contains reference cycles.

!!! warning "json does not support `number` indices"
	This function will convert all `number` indices to `string` indices. For example, `[1]` is converted to `["1"]`. This should be taken into account when loading/decoding json files.
]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "object", type = "table" },
		{ name = "state", type = "table?" },
	},
	valuetype = "string",
}
