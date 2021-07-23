return {
	type = "function",
	description = [[Create a simple capture pattern, which captures the substring of the subject that matches pattern p.
	
The captured value is a string. If p has other captures, their values are returned after this one.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-c",
	arguments = {
		{ name = "p", type = "pattern", description = "The pattern to capture against." },
	},
	valuetype = "pattern",
}