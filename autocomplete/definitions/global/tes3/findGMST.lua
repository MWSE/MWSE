return {
	type = "function",
	description = [[Fetches the core game object that represents a game setting. While this function accepts a name, it is recommended to use the [`tes3.GMST`](https://mwse.github.io/MWSE/references/gmst/) constants.]],
	arguments = {
		{ name = "id", type = "tes3.gmst|integer|string" },
	},
	returns = { name = "gameSetting", type = "tes3gameSetting?" },
	examples = {
		["retrieveGMST"] = {
			title = "Retrieve value of a GMST",
			description = "This example shows how to read a value of a GMST and how to change it.",
		},
		["markdown"] = {
			title = "Document all GMST Default Values",
			description = "This example reads the default values of all GMSTs, and writes them to a markdown file for users to read.",
		},
	},
}
