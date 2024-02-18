return {
	type = "function",
	description = [[Create a string representing the object. Object can be a table, a string, a number, a boolean, nil, json.null or any object with a function __tojson in its metatable. A table can only use strings and numbers as keys and its values have to be valid objects as well. It raises an error for any invalid data types or reference cycles.

!!! warning
	If the table being encoded has both string and integer indices, this action will convert all the integer indices to strings. For example, `[1]` is converted to `["1"]`. This should be taken into account when loading json files.
]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "object", type = "table" },
		{ name = "state", type = "table?" },
	},
	valuetype = "string",
}
