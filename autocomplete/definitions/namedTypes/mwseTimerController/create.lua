return {
	type = "method",
	description = [[Creates a timer for the given Timer Controller.

!!! tip
	It's recommended to study the [Object Lifetimes](../guides/object-lifetimes.md) guide. It describes how to safely use [tes3reference](../types/tes3reference.md) objects inside timer callbacks.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "duration", type = "number", description = "Duration of the timer. The method of time passing depends on the timer type." },
			{ name = "callback", type = "fun(e: mwseTimerCallbackData)|string", description = "The callback function that will execute when the timer expires. If starting a registered timer, this needs to be the `name` string passed to `timer.register`." },
			{ name = "iterations", type = "integer", optional = true, default = 1, description = "The number of iterations to run. Use `-1` for infinite looping." },
			{ name = "persist", type = "boolean", optional = true, default = true, description = "Registering a timer with persist flag set to `true` will serialize the callback string in the save to persist between sessions. Only a registered timer will persist between sessions. See `timer.register()`." },
			{ name = "data", type = "table", optional = true, description = "Data to be attached to the timer. If this is a persistent timer, the data must be json-serializable, matching the same limitations as data stored on references." },
		}
	}},
	returns = { name = "timer", type = "mwseTimer" }
}
