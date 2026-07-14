return {
	type = "function",
	description = "Calls arrays of event handlers until one group handles the event.",
	arguments = {
		{ name = "handlers", type = "(fun(...): boolean?)[][]", description = "The handler groups to call." },
		{ name = "...", type = "any", description = "Arguments passed to each handler." },
	},
	returns = {
		{ name = "eventHandled", type = "boolean", description = "True if no further handler groups should be called." },
	},
}
