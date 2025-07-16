return {
	type = "method",
	description = [[Searches the parent node chain returning the node that matches the argument.]],
	arguments = {
		{ name = "name", type = "string" },
	},
	returns = { name = "parentNode", type = "niNode|nil" },
}