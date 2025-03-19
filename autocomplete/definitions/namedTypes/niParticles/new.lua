return {
	type = "function",
	description = [[Creates a new niParticles.]],
	arguments = {
		{ name = "vertexCount", type = "number", description = "The number of particles." },
		{ name = "hasNormals", type = "boolean", description = "If `true`, normals will be allocated." },
		{ name = "hasColors", type = "boolean", description = "If `true`, colors will be allocated." },
	},
	returns = {
		{ name = "particles", type = "niParticles" },
	},
}
