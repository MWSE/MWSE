return {
	type = "method",
	description = [[Calculates the distance to another vector, using the [Manhattan (i.e. city block) metric](https://en.wikipedia.org/wiki/Taxicab_geometry). 
In the two-dimensional case, the Manhattan metric can be thought of 
as the distance that two taxis will have to travel if they're following a grid system.
The formula for the Manhattan distance is

	math.abs(v1.x - v2.x) + math.abs(v1.y - v2.y) + math.abs(v1.z - v2.z)

This is useful for checking how far you'd actually have to move if you're only allowed to move along one axis at a time.
]],
	arguments = {
		{ name = "vec", type = "tes3vector3" },
	},
	valuetype = "number",
}