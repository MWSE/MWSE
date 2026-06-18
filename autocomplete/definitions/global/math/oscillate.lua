return {
	type = "function",
	description = "Bounces a phase value back and forth between the inclusive range `inMin` to `inMax`.",
	arguments = {
		{ name = "phase", type = "number" },
		{ name = "inMin", type = "number" },
		{ name = "inMax", type = "number", description = "Must differ from `inMin`." },
	},
	valuetype = "number",
}
