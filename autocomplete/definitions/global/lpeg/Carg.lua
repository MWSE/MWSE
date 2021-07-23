return {
	type = "function",
	description = [[Create an argument capture.
	
This pattern matches the empty string and produces the value given as the nth extra argument given 
in the call to lpeg.match.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-arg",
	arguments = {
		{ name = "n", type = "number", description = "The argument to produce." },
	},
	valuetype = "pattern",
}