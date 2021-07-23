return {
	type = "function",
	description = [[Create a substitution capture, which captures the substring of the subject that matches pattern p, with substitutions.
	
For any capture inside p with a value, the substring that matched the capture is replaced by the 
capture value (which should be a string). The final captured value is the string resulting from all
replacements.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-s",
	arguments = {
		{ name = "p", type = "pattern", description = "The pattern to capture with." },
	},
	valuetype = "pattern",
}