return {
	type = "method",
	description = "Gets all the magic effect instances for a given effect index. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.",
	arguments = {
		{ name = "index", type = "number", description = "The index in the effect list to fetch, between `0` and `7`." },
	},
	valuetype = "tes3magicEffectInstance[]",
}