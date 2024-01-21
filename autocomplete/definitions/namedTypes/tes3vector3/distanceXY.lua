return {
	type = "method",
	description = [[Calculates the distance between the XY-coordinates of two vectors.

This method offers a way of calculating distances between vectors in situations where it's more convenient to ignore the z-coordinates.
]],
	arguments = {
		{ name = "vec", type = "tes3vector3" },
	},
	valuetype = "number",
	examples={
		["shelves"] = {
			title = "Items on bookshelves.",
			description = "\z
				Let's say you want to make a function that checks if two ingredients are close together. \z
				This will involve looking at the distance between two `tes3reference`s.\n\z
				\z
				One way to do this would be to use the normal `tes3vector3:distance` method, but this has a drawback: \z
				it doesn't work consistently with ingredients on bookshelves. \z
				If two ingredients are on the same shelf, their `z`-coordinates contribute very little to the distance between them, while the situation is reversed \z
				for ingredients on different shelves.\n\z
				This problem is remedied by using `tes3vector3:distanceXY` as follows:\z
			",
				-- \z
				-- Let's say you want to test whether two `tes3reference`s are close together. You could do this by typing \z
				-- `pos1:distance(pos2) < maxDist`, but this has a drawback: it doesn't properly handle scenarios where  on bookshelves. \z
				-- To be more specific, if you compared an ingredient on a bottom shelf to an ingredient on the top shelf, the difference in their z-coordinates \z
				-- could be greater than `maxDist`. Here's how you might solve that problem.\z
			-- ",
		},
	},
}