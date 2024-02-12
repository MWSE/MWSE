return {
	type = "function",
	description = [[Returns a string where one string has been inserted into another, after a specified position.
		
For example, `string.insert("12345678", "abcdefgh", 5)` will return `"12345abcdefgh678"`.]],
	arguments = {
		{ name = "s1", type = "string", description = "The string to insert into." },
		{ name = "s2", type = "string", description = "The string to insert." },
		{ name = "position", type = "integer", description = "An index of `s1`. The `s2` `string` will be inserted after this index." },
	},
	returns = {
		{ name = "result", type = "string", description = "A copy of `s1`, with `s2` inserted into it after the specified `position`." },
	}
}