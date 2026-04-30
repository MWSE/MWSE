return {
	type = "function",
	description = "Creates a new niPackedColor object. This function must be given 0, 3, or 4 arguments.",
	arguments = {
		{ name = "r", type = "integer", description = "The red value for the color.", optional = true, default = 0 },
		{ name = "g", type = "integer", description = "The green value for the color.", optional = true, default = 0 },
		{ name = "b", type = "integer", description = "The blue value for the color.", optional = true, default = 0 },
		{ name = "a", type = "integer", description = "The alpha value for the color.", optional = true, default = 0 },
	},
	returns = {
		{ name = "color", type = "niPackedColor" },
	}
}
