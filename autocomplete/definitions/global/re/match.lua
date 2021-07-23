return {
	type = "function",
	description = [[Matches the given pattern against the given subject, returning all captures.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/re.html",
	arguments = {
		{ name = "subject", type = "string", description = "The string to search."},
		{ name = "pattern", type = "string", description = "The pattern to search with."},
	},
	valuetype = "string",
	examples = {
		["SimpleMatch"] = {
			title = "Simple Match",
			description = "Use re.match to find all the words in a string, then find the first numeral.",
		},
	},
}