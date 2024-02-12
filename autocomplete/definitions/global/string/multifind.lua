return {
	type = "function",
	description = [[Performs the logic of `string.find` on a `string` `s`, using a `table` of patterns.

If any of the `patterns` are found in `s`, then the matching `pattern` will be returned, followed by the normal results of `string.find`.
	
The `patterns` are checked in the order they are passed. i.e., this function will first try to match `patterns[1]`, then `patterns[2]`, and so on.]],
	arguments = {
		{ name = "s", type = "string", description = "The `string` to `find` `patterns` in." },
		{ name = "patterns", type = "table", description = "An array-style `table` that contains the patterns to match." },
		{ name = "index", type = "integer", optional = true, default = 1, description = "Start index of the `find`. (Same meaning as in `string.find`.)" },
		{ name = "plain", type = "boolean", optional = true, default = false, description = "If `true`, then a normal search will be performed instead of a pattern search. (Same meaning as in `string.find`.)" },
	},
	returns = {
		{ name = "pattern", type = "string", optional = true, 
			description = "If a pattern was matched, then this will be the first pattern that was matched. If no patterns matched, this will be `nil`."
		},
		{ name = "startindex", type = "integer", description = "If a `pattern` was matched, this is the index of `s` where the matching `pattern` begins.", optional = true },
		{ name = "endindex", type = "integer", description = "If a `pattern` was matched, this is the index of `s` where the matching `pattern` ends.", optional = true },
	},
}