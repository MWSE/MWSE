return {
	type = "function",
	description = [[Checks if there is a clear line of sight between two references, or two points in space, in the same method that the game uses for actor vision. You should pass two references, or two positions and heights.

Only certain object types count as blocking for the purposes of the test:
Statics, activators, containers, doors, and lights with non-zero weight.

The collision root node is used for testing, if present in the model.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference1", type = "tes3reference", description = "Position of the starting point of the LoS check. For actors, this point is set to position of this reference's head. For other objects the starting point is at the top of its bounding box." },
			{ name = "reference2", type = "tes3reference", description = "Position of the ending point of the LoS check. For actors, this point is set to position of this reference's head. For other objects the ending point is at the top of its bounding box." },
			{ name = "position1", type = "tes3vector3|table", description = "Position of the starting point of the LoS check. Modified by height1." },
			{ name = "height1", optional = true, type = "tes3vector3|table", description = "Moves the starting point upwards (+Z direction) by this amount. Normally used to simulate head height from a position that is on the ground. Defaults to 0." },
			{ name = "position2", type = "tes3vector3|table", description = "Position of the ending point of the LoS check. Modified by height2." },
			{ name = "height2", optional = true, type = "tes3vector3|table", description = "Moves the starting point upwards (+Z direction) by this amount. Normally used to simulate head height from a position that is on the ground. Defaults to 0." },
		},
	}},
	returns = "hasLineOfSight",
	valuetype = "boolean",
}
