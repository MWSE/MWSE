return {
	type = "method",
	description = "Gets the magic effect instance for a given effect index and target reference. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.",
	arguments = {
		{ name = "index", type = "number", description = "The index in the effect list to fetch, between 0 and 7." },
		{ name = "target", type = "tes3reference", description = "The target actor for the effect." },
	},
	valuetype = "tes3magicEffectInstance",
}