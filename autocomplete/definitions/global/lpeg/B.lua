return {
	type = "function",
	description = [[Returns a pattern that matches only if the input string at the current position is preceded by pattern p.
	
Pattern p must match only strings with some fixed length, and it cannot contain captures.

Like the and predicate, this pattern never consumes any input, independently of success or failure.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#op-behind",
	arguments = {
		{ name = "p", type = "pattern", description = "The input pattern." },
	},
	valuetype = "pattern",
}