return {
	type = "method",
	description = [[Gets the one-based position of the tab with the given ID.]],
	arguments = {
		{ name = "id", type = "string", description = "The unique identifier for the tab." },
	},
	returns = { name = "position", type = "number|nil" },
}
