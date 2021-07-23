return {
	type = "method",
	description = [[Calculates the interpolated vector between this vector and another, given a transition.]],
	arguments = {
		{ name = "toVector", type = "tes3vector3", description = "The vector to interpolate towards." },
		{ name = "transition", type = "number", description = "The interpolation value. Must be between 0.0 (closer to this vector) and 1.0 (closer to the other vector)." },
	},
	returns = {
		{ name = "lerpedVector", type = "tes3vector3", description = "The calculated value." },
	},
}