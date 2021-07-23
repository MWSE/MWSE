return {
	type = "function",
	description = [[Returns a value from Morrowind's random number generator. This is not preferrable to lua's math.random function, but may be necessary for reproducing Morrowind's generation.]],
	arguments = {
		{ name = "seed", type = "number", optional = true, description = "If provided, it the number generator is seeded with this value. Pointers to objects may be used, such as a reference's sceneNode, to create a consistent if less than random seed." },
	},
}