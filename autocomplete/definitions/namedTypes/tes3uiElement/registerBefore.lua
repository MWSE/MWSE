return {
	type = "method",
	description = [[Sets an ``event`` handler to run before any existing event handler on the element. Can be any event usable with ``register``. The callback receives an argument with the event data. See ``register`` for details.]],
	arguments = {
		{ name = "eventID", type = "string", description = "The event id." },
		{ name = "callback", type = "function", description = "The callback function." },
	},
}