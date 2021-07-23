return {
	type = "method",
	description = [[Performs the picking operation.]],
	arguments = {
		{ name = "origin", type = "tes3vector3" },
		{ name = "distance", type = "tes3vector3" },
		{ name = "append", type = "boolean", optional = true, description = "Default value: false" },
		{ name = "maxDistance", type = "number", optional = true, description = "Default value: 0.0F" },
	},
}