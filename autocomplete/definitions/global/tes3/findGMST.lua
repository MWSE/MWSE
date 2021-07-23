return {
	type = "function",
	description = [[Fetches the core game object that represents a game setting. While this function accepts a name, it is recommended to use the tes3.GMST constants.]],
	arguments = {
		{ name = "id", type = "number|string" },
	},
	returns = "gameSetting",
	valuetype = "tes3gameSetting",
    examples = {
		["retrieveGMST"] = {
			title = "Retrieve value of a GMST",
			description = "This example shows how to read a value of a GMST and how to change it.",
		},
	},
}
