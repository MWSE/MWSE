return {
	type = "function",
	description = [[Creates a timer that will finish the next frame. It defaults to the next simulation frame.

!!! tip
	It's recommended to study the [Object Lifetimes](../guides/object-lifetimes.md) guide. It describes how to safely use [tes3reference](../types/tes3reference.md) objects inside timer callbacks.]],
	arguments = {
		{ name = "callback", type = "fun(e: mwseTimerCallbackData)", description = "The callback function that will execute when the timer expires." },
		{ name = "type", type = "integer|mwseTimer.type", optional = true, default = "`timer.simulate`", description = "Type of the timer. This value can be `timer.simulate`, `timer.game` or `timer.real`." },
	},
	returns = { name = "timer", type = "mwseTimer" },
}
