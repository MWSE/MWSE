return {
	type = "function",
	description = [[Creates a new niAutoNormalParticles.]],
	arguments = {
		{ name = "vertexCount", type = "number", description = "The number of particles." },
		{ name = "hasColors", type = "boolean", description = "If `true`, colors will be allocated." },
	},
	returns = {
		{ name = "particles", type = "niAutoNormalParticles" },
	},
}
