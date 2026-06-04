return {
	type = "method",
	description = [[Sets whether the tab with the given ID is disabled. If the selected tab is disabled, the next available tab will be selected.]],
	arguments = {
		{ name = "id", type = "string", description = "The unique identifier for the tab." },
		{ name = "disabled", type = "boolean", description = "If true, the tab will be disabled." },
	},
}
