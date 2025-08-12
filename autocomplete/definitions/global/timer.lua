return {
	type = "lib",
	description = [[The timer library provides helper functions for creating delayed executors.

!!! warning "Timers get canceled when loading saves."
	All active timers will be canceled right before the [`loaded`](../events/loaded.md) event triggers. Consider using a [persistent timer](https://mwse.github.io/MWSE/apis/timer/#timerregister) if you need it to persist between game sessions.
]],
}