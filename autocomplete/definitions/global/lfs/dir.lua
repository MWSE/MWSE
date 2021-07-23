return {
	type = "function",
	description = [[Creates a Lua iterator over the entries of a given directory. This can be used to create a for loop of directories. Be wary that current and parent directory markers may be available in the results.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to iterate over." },
	},
}