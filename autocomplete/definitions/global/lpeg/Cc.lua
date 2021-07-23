return {
	type = "function",
	description = [[Create a constant capture.
	
This pattern matches the empty string and produces all given values as its captured values.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-cc",
	arguments = {
		{ type = "values", optional = true, description = "The values to produce." },
	},
	valuetype = "pattern",
}