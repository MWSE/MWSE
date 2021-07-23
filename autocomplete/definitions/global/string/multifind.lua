return {
	type = "function",
	description = [[Performs the logic of find, using a table of patterns.

If any of the available patterns match, the matching pattern is returned followed by the normal results of the find.]],
	arguments = {
		{ name = "s", type = "string", description = "The string to perform finds on." },
		{ name = "pattern", type = "table", description = "An array-style table that contains pattern strings." },
		{ name = "index", type = "number", default = 1, description = "Start index of the find." },
		{ name = "plain", type = "boolean", default = false, description = "If true, a normal search instead of a pattern search will be performed." },
	},
}