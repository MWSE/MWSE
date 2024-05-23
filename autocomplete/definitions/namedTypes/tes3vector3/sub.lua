return {
	type = "operator",
	overloads = {
		{ rightType = "number", resultType = "tes3vector3", description = "Subtracts given number from each of the vector's components." },
		{ rightType = "tes3vector3", resultType = "tes3vector3", description = "Standard vector subtraction." },
	}
}
