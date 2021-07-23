return {
	type = "function",
	description = [[Create a table capture.
	
This capture returns a table with all values from all anonymous captures made by pattern p inside 
this table in successive integer keys, starting at 1. Moreover, for each named capture group 
created by p, the first value of the group is put into the table with the group name as its key. 
The captured value is only the table.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-t",
	arguments = {
		{ name = "p", type = "pattern", description = "The pattern to capture with." },
	},
	valuetype = "pattern",
}