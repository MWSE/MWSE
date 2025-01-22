return {
	type = "function",
	description = [[Creates a timer.
!!! warning "Timers get canceled when loading saves."
	All active timers will be canceled right before the [`loaded`](../events/loaded.md) event triggers.
	
!!! tip
	It's recommended to study the [Object Lifetimes](../guides/object-lifetimes.md) guide. It describes how to safely use [tes3reference](../types/tes3reference.md) objects inside timer callbacks.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "type", type = "integer", optional = true, default = "`timer.simulate`", description = "Type of the timer. This value can be `timer.simulate`, `timer.game` or `timer.real`." },
			{ name = "duration", type = "number", description = "Duration of the timer. The method of time passing depends on the timer type." },
			{ name = "callback", type = "fun(e: mwseTimerCallbackData)|string", description = "The callback function that will execute when the timer expires. If starting a registered timer, this needs to be the `name` string passed to `timer.register`." },
			{ name = "iterations", type = "integer", optional = true, default = 1, description = "The number of iterations to run. Use `-1` for infinite looping." },
			{ name = "persist", type = "boolean", optional = true, default = true, description = "Registering a timer with persist flag set to `true` will serialize the callback string in the save to persist between sessions. Only a registered timer will persist between sessions. See `timer.register()`." },
			{ name = "data", type = "table|nil", optional = true, default = "nil", description = "Data to be attached to the timer. If this is a persistent timer, the data must be json-serializable, matching the same limitations as data stored on references." },
		}
	}},
	returns = { name = "timer", type = "mwseTimer" },
	examples = {
		["ShowCountdownMessage"] = {
			title = "Show a Countdown Message",
			description = "In this example, whenever the player activates an item they will be shown a 10 second countdown. Because it is a simulation timer, it will not tick down while the menu is open, nor is it sensitive to the game timescale/clock.",
		},
	},
}
