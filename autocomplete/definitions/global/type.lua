return {
	type = "function",
	description = [[Returns the type of a given lua variable. It also provides the name of userdata variables.]],
	arguments = {
		{ name = "variable", type = "any", description = "The variable to get the type of." },
	},
	returns = {
		{ name = "type", type = "string", description = "The base type of the variable." },
		{ name = "userdataType", type = "string|nil", description = "The userdata type of the variable." },
	},
}