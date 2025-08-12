return {
	type = "method",
	description = [[Searches the `children` array for a node with a name that matches the argument and returns the index of found node. If no node with given name is found returns `-1`.]],
	arguments = {
		{ name = "name", type = "string" },
	},
	returns = { name = "childIndex", type = "integer" },
}