return {
	type = "function",
	description = [[Does a global substitution, replacing all occurrences of pattern in the given subject by replacement.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/re.html",
	arguments = {
		{ name = "subject", type = "string", description = "The string to search."},
		{ name = "pattern", type = "string", description = "The pattern to search with."},
		{ name = "replacement", type = "string", description = "Replace all matches with this string." },
	},
	valuetype = "string",
	examples = {
		["SimpleSubstitution"] = {
			title = "Simple Substitution",
			description = "Use re.gsub to replace all vowels in a string with '.'.",
		},
	},
}