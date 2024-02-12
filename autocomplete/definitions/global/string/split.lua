return {
	type = "function",
	description = [[Returns an array-style table with a `string` split by a specified separator.
The seperator is not part of the results. 
By default the `sep == "%s"`, which will result in `str` getting split by whitespace characters (e.g. spaces and tabs).]],
	arguments = {
		{ name = "str", type = "string", description = "The string to split." },
		{ name = "sep", type = "string", optional = true, default = [["%s"]], description = "The token to split the string by." },
	},
	returns = {
		{ name = "split", type = "string[]" },
	},
}
