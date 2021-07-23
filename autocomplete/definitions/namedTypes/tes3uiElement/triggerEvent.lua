return {
	type = "method",
	description = [[Triggers a UI event on an element, either using supplied event data, or by constructing new event data using ``eventName``. ``eventName`` is the same as used in ``register``.]],
	arguments = {
		{ name = "eventID", type = "tes3uiEvent|string", description = "The event, or event ID." },
	},
}