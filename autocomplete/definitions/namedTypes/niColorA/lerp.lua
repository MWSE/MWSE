return {
	type = "method",
	description = [[Calculates the interpolated color between this color and another, given a transition.]],
	arguments = {
		{ name = "tocolor", type = "niColorA", description = "The color to interpolate towards." },
		{ name = "transition", type = "number", description = "The interpolation value. Must be between 0.0 (closer to this color) and 1.0 (closer to the other color)." },
	},
	returns = {
		{ name = "lerpedcolor", type = "niColorA", description = "The calculated value." },
	},
}