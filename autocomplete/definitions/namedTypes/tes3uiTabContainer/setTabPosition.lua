return {
	type = "method",
	description = [[Moves the tab with the given ID to a new one-based position. The associated contents block will be moved to the same position.]],
	arguments = {
		{ name = "id", type = "string", description = "The unique identifier for the tab." },
		{ name = "position", type = "number", description = "The one-based position to move the tab to." },
	},
}
