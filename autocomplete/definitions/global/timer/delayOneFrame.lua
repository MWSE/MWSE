return {
	type = "function",
	description = [[Creates a timer that will finish the next frame. It defaults to the next simulation frame.]],
	arguments = {
		{ name = "callback", type = "function", description = "The callback function that will execute when the timer expires." },
		{ name = "type", type = "number", optional = true, default = "timer.simulate", description = "Type of the timer. This value can be timer.simulate, timer.game or timer.real." },
	},
	returns = "timer",
	valuetype = "mwseTimer",
}
