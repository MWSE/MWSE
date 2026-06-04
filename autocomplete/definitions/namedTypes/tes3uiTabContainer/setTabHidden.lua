return {
	type = "method",
	description = [[Sets whether the tab with the given ID is hidden. If the selected tab is hidden, the next available tab will be selected.]],
	arguments = {
		{ name = "id", type = "string", description = "The unique identifier for the tab." },
		{ name = "hidden", type = "boolean", description = "If true, the tab will be hidden." },
	},
}
