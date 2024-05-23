return {
	type = "function",
	description = [[Returns an array-style table with a `string` split by a specified separator.
The seperator is not part of the results. 
By default the `sep == "%s"`, which will result in `str` getting split by whitespace characters (e.g. spaces and tabs).

!!! warning 
	if `sep` is more than one character, then `str` will get split at each occurrence of any of the individual characters in `sep`.
	For example, `string.split("1a2b3c4abc5", "abc")` will return `{"1", "2", "3", "4", "5"}` instead of `{"1a2b3c4", "5"}`.
]],
	arguments = {
		{ name = "str", type = "string", description = "The string to split." },
		{ name = "sep", type = "string", optional = true, default = [["%s"]], description = "The token to split the string by." },
	},
	returns = {
		{ name = "split", type = "string[]" },
	},
}
