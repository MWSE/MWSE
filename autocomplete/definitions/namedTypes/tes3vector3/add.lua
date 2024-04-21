return {
	type = "operator",
	overloads = {
		{ rightType = "number", resultType = "tes3vector3", description = "Add the given number to each of the vector's components." },
		{ rightType = "tes3vector3", resultType = "tes3vector3", description = "Standard vector addition." },
	}
}
