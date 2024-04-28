return {
	type = "method",
	description = [[Unregisters a function previously registered using `:registerBefore`.]],
	arguments = {
		{ name = "eventID", type = "tes3.uiEvent", description = "The event id." },
		{ name = "callback", type = "integer|fun(e: tes3uiEventData): boolean?", description = "The callback function." },
	},
	returns = {
		{ name = "wasUnregistered", type = "boolean" },
	},
}