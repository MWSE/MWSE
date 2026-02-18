return {
	type = "method",
	description = [[Clamps a given point into the bounding box, given an origin.]],
	arguments = {
		{ name = "point", type = "tes3vector3", description = "The point to clamp. It is modified as part of this method call." },
		{ name = "origin", type = "tes3vector3", description = "The origin to offset the minimum/maximum vectors by." },
	},
	valuetype = "boolean",
}
