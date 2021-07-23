return {
	type = "function",
	description = [[Attempt to match the given pattern against the subject string.
	
If the match succeeds, return the index in the subject of the first character after the match, or
the captured values (if the pattern captured any value).

An optional numeric argument init makes the match start at that position in the subject string. As 
usual in Lua libraries, a negative value counts from the end.

Unlike typical pattern-matching functions, match works only in anchored mode; that is, it tries to 
match the pattern with a prefix of the given subject string (at position init), not with an 
arbitrary substring of the subject. So, if we want to find a pattern anywhere in a string, we must
either write a loop in Lua or write a pattern that matches anywhere. This second approach is easy 
and quite efficient; see examples in the full documentation for details.]],
	link = "http://www.inf.puc-rio.br/~roberto/lpeg/#f-match",
	arguments = {
		{ name = "p", type = "pattern", description = "The pattern to match."},
		{ name = "subject", type = "string", description = "The string to match against." },
		{ name = "init", type = "number", optional = true, description = "Start the match at this position in subject." },
	},
	valuetype = "values",
	examples = {
		["SimpleMatch"] = {
			title = "Simple Match",
			description = "Create a pattern that matches a word followed by the end of the string, and use it to match strings.",
		},
	},
}