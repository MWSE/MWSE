return {
	type = "function",
	description = "Creates a new notify menu with a formatted string. A notify menu is a toast-style display that shows at the bottom of the screen. It will expire after an amount of time, determined by the length of the message and the `fMessageTimePerChar` GMST.",
	arguments = {
		{ name = "string", type = "string", description = "The message to display. If it supports formatting, additional arguments are used." },
		{ type = "variadic", description="Formatting arguments. These are passed to `string.format`.", optional = true }
	},
	returns = {
		{ name = "menu", type = "tes3uiElement", description = "The notify menu created." },
	}
}
