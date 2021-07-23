return {
	type = "function",
	description = [[Creates a timer.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "type", type = "number", optional = true, default = "timer.simulate", description = "Type of the timer. This value can be timer.simulate, timer.game or timer.real." },
			{ name = "duration", type = "number", description = "Duration of the timer. The method of time passing depends on the timer type." },
			{ name = "callback", type = "function", description = "The callback function that will execute when the timer expires." },
			{ name = "iterations", type = "number", optional = true, default = 1, description = "The number of iterations to run. Use ``-1`` for infinite looping." },
		}
	}},
	returns = "timer",
	valuetype = "mwseTimer",
	examples = {
		["ShowCountdownMessage"] = {
			title = "Show a Countdown Message",
			description = "In this example, whenever the player activates an item they will be shown a 10 second countdown. Because it is a simulation timer, it will not tick down while the menu is open, nor is it sensitive to the game timescale/clock.",
		},
	},
}
