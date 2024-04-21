return {
	type = "method",
	description = [[Unregisters an `event` handler.]],
	arguments = {
		{ name = "eventID", type = "tes3.uiEvent", description = "The event id." },
	},
	returns = {
		{ name = "wasUnregistered", type = "boolean" },
	},
}