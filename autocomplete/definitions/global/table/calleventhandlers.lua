return {
	type = "function",
	description = "Calls event handlers in reverse order until one returns `false`.",
	arguments = {
		{ name = "handlers", type = "(fun(...): boolean?)[]", description = "The event handlers to call." },
		{ name = "...", type = "any", description = "Arguments passed to each handler." },
	},
	returns = {
		{ name = "eventHandled", type = "boolean", description = "True if no further handlers should be called." },
	},
}
