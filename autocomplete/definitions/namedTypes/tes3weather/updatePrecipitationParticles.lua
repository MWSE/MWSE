return {
	type = "method",
	description = [[Updates precipitation particles for the given weather particle type. This handles spawn probability, active particle count, and particle creation using the provided spawn region settings.]],
	arguments = {
		{ name = "type", type = "number", description = "The precipitation particle type to update. `1` is rain and `5` is snow." },
		{ name = "transitionScalar", type = "number", description = "The weather's current transition scalar." },
		{ name = "deltaTime", type = "number", description = "The frame delta time." },
		{ name = "radius", type = "number", description = "The spawn radius for new particles." },
		{ name = "heightMin", type = "number", description = "The minimum spawn height for new particles." },
		{ name = "heightMax", type = "number", description = "The maximum spawn height for new particles." },
		{ name = "entranceSpeed", type = "number", description = "The particle spawn rate scalar. Higher values spawn more slowly." },
	},
}
