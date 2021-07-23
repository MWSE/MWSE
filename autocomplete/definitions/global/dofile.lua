return {
	type = "function",
	description = [[Reads, loads, and excutes a lua file at a given path. Similar to the require and include functions, dofile allows the use of periods as a path delimiter. Unlike require, a path relative to Morrowind.exe may be given.]],
	arguments = {
		{ name = "path", type = "string" },
	},
}