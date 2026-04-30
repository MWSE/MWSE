return {
	type = "function",
	description = "Creates a new niColorA. This function must be given 0, 3, or 4 arguments.",
	arguments = {
		{ name = "r", type = "number", description = "The red value for the color.", optional = true, default = 0 },
		{ name = "g", type = "number", description = "The green value for the color.", optional = true, default = 0 },
		{ name = "b", type = "number", description = "The blue value for the color.", optional = true, default = 0 },
		{ name = "a", type = "number", description = "The alpha value for the color.", optional = true, default = 0 },
	},
	returns = {
		{ name = "color", type = "niColorA" },
	}
}
