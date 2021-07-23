return {
	type = "function",
	description = [[Create a group capture.
	
It groups all values returned by pattern p into a single capture. The group may be anonymous (if no
name is given) or named with the given name (which can be any non-nil Lua value).

An anonymous group serves to join values from several captures into a single capture. A named 
group has a different behavior. In most situations, a named group returns no values at all. Its 
values are only relevant for a following back capture or when used inside a table capture.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#cap-g",
	arguments = {
		{ name = "p", type = "pattern", description = "The pattern to capture with." },
		{ name = "name", type = "value", optional = true, description = "The name of the group." },
	},
	valuetype = "pattern",
}