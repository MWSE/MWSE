return {
	type = "function",
	description = [[Searches the given pattern in the given subject.
	
If it finds a match, returns the index where this occurrence starts and the index where it ends. 
Otherwise, returns nil.

An optional numeric argument init makes the search starts at that position in the subject string.
As usual in Lua libraries, a negative value counts from the end. ]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/re.html",
	arguments = {
		{ name = "subject", type = "string", description = "The string to search."},
		{ name = "pattern", type = "string", description = "The pattern to search with."},
		{ name = "init", type = "number", optional = true, description = "Start at this position in the subject string." },
	},
	valuetype = "number",
	examples = {
		["SimpleFind"] = {
			title = "Simple Find",
			description = "Use re.find to get the position of the first numeral in a string.",
		},
	},
}