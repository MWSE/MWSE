return {
	type = "method",
	description = [[Fill the quaternion by converting an angle-axis rotation. The angle must be within the interval [0, PI] and the axis must be unit length.]],
	arguments = {
		{ name = "angle", type = "float" },
		{ name = "axis", type = "tes3vector3" },
	},
}
