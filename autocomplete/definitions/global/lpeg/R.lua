return {
	type = "function",
	description = [[Returns a pattern that matches any single character belonging to one of the given ranges.
	
Each range is a string xy of length 2, representing all characters with code between the codes of x and y (both inclusive).

As an example, the pattern lpeg.R("09") matches any digit, and lpeg.R("az", "AZ") matches any ASCII letter.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#op-r",
	arguments = {
		{ name = "p", type = "pattern", description = "The input pattern." },
	},
	valuetype = "pattern",
	examples = {
		["SimpleMatch"] = {
			title = "Simple Match",
			description = "Create a pattern using lpeg.R that matches a word followed by the end of the string.",
		},
	},
}