return {
	type = "function",
	description = [[This function compares `a` and `b` and returns true if they are close together. This can be useful when comparing floating-point numbers with some degree of tolerance.]],
	arguments = {
		{ name = "a", type = "number", description = "First value." },
		{ name = "b", type = "number", description = "Second value." },
		{ name = "absoluteTolerance", type = "number", optional = true, default = "math.epsilon", description = "The absolute difference allowed between the two numbers. A value of 0.01 will only allow the values to differ by 0.01." },
		{ name = "relativeTolerance", type = "number", optional = true, default = 1e-9, description = "The relative difference allowed between the two numbers. A value of 0.01 will only allow the values to differ by 1%." },
	},
	valuetype = "boolean",
}
