return {
	type = "method",
	description = [[Calculates the distance between the XY-coordinates of two vectors.

This method offers a way of calculating distances between vectors in situations where it's more convenient to ignore the z-coordinates.
]],
	arguments = {
		{ name = "vec", type = "tes3vector3" },
	},
	valuetype = "number",
	examples = {
		["shelves"] = {
			title = "Items on bookshelves.",
			description = "\z
				Let's say you want to make a function that checks if two ingredients are close together. \z
				This will involve looking at the distance between two `tes3reference`s.\n\n\z
				\z
				One way to do this would be to use the normal `tes3vector3:distance` method, but this has a drawback: \z
				it doesn't work consistently with ingredients on bookshelves. \z
				If two ingredients are on the same shelf, their `z`-coordinates contribute very little to the distance between them, while the situation is reversed \z
				for ingredients on different shelves.\n\n\z
				This problem is remedied by using `tes3vector3:distanceXY` as follows:\z
			",
		},
	},
}