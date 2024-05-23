return {
	type = "operator",
	overloads = {
		{ rightType = "tes3vector2", resultType = "tes3vector2", description = "The per-element multiplication of two vectors, also known as [Hadamard product](https://en.wikipedia.org/wiki/Hadamard_product_(matrices))." },
		{ rightType = "number", resultType = "tes3vector2", description = "Multiplies the vector by a scalar." },
	}
}
