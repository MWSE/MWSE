return {
	type = "method",
	description = [[Causes the actor to look towards this reference, while obey the usual head turning constraints. This must be called every frame in the `simulate` event to work. It will override regular head look behaviour and the target may be at any distance in the same worldspace.]],
	arguments = {
		{ name = "target", type = "tes3reference" },
	},
	experimental = true,
}