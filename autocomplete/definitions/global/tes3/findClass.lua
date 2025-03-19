return {
	type = "function",
	description = [[Fetches the core game character class object for a given class ID. If the class with a given ID doesn't exist, nil is returned.]],
	arguments = {
		{ name = "id", type = "string", description = "ID of the class to search for." },
	},
	returns = "class",
	valuetype = "tes3class?",
}
