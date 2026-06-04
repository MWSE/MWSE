return {
	type = "method",
	description = [[Gets the contents block for the tab with the given ID.]],
	arguments = {
		{ name = "id", type = "string", description = "The unique identifier for the tab." },
	},
	returns = { name = "contents", type = "tes3uiElement|nil" },
}
