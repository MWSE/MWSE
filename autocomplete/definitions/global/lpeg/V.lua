return {
	type = "function",
	description = [[Create a non-terminal (a variable) for a grammar.
	
The created non-terminal refers to the rule indexed by v in the enclosing grammar. See Grammars
in the full documentation for details.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#op-v",
	arguments = {
		{ name = "v", type = "string", description = "The index of the desired rule." },
	},
	valuetype = "pattern",
}