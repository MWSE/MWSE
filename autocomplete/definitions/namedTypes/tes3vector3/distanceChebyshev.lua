return {
	type = "method",
	description = [[Calculates the distance to another vector, using the Chebyshev metric, which is defined as

	math.max(math.abs(v1.x - v2.x), math.abs(v1.y - v2.y), math.abs(v1.z - v2.z))

This is useful for ensuring that the x, y, and z coordinates between two vectors are all (independently) within a certain distance from each other.

Here is a geometric description of the difference between the normal distance and the Chebyshev distance for two `tes3vector3`s  `v1` and `v2`:
* If `v1:distance(v2) <= 1`, then `v2` is contained in a sphere around `v1` with radius 1 (i.e. diameter 2).
* If `v1:distanceChebyshev(v2) <= 1`, then `v2` is contained within a cube centered around `v1`, where the cube has length 2.
]],
	arguments = {
		{ name = "vec", type = "tes3vector3" },
	},
	valuetype = "number",
}