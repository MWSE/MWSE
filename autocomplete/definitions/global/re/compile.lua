return {
	type = "function",
	description = [[Compiles the given string and returns an equivalent lpeg pattern.
	
The given string may define either an expression or a grammar. The optional defs table provides 
extra Lua values to be used by the pattern.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/re.html",
	arguments = {
		{ name = "s", type = "string", description = "The string to compile."},
		{ name = "defs", type = "table", optional = true, description = "Extra lua values to be used by the pattern." },
	},
	valuetype = "pattern",
}