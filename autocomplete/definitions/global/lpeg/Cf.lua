return {
	type = "function",
	description = [[Create a fold capture.
	
If pattern p produces a list of captures C1 C2 ... Cn, this capture will produce the value 
function f(...f(f(C1, C2), C3)..., Cn), that is, it will fold (or accumulate, or reduce) the 
captures from p using function f.

This capture assumes that p should produce at least one capture with at least one value (of any 
type), which becomes the initial value of an accumulator. (If you need a specific initial value, 
you may prefix a constant capture to p.) For each subsequent capture, LPeg calls f with this
accumulator as the first argument and all values produced by the capture as extra arguments; the 
first result from this call becomes the new value for the accumulator. The final value of the 
accumulator becomes the captured value.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-f",
	arguments = {
		{ name = "p", type = "pattern", description = "The pattern to fold with f." },
		{ name = "f", type = "function", description = "The function to use to fold p." },
	},
	valuetype = "pattern",
}