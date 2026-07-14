return {
	type = "function",
	description = "Returns smooth cubic interpolation over the range `edge0` to `edge1`.",
	arguments = {
		{ name = "edge0", type = "number" },
		{ name = "edge1", type = "number", description = "Must differ from `edge0`." },
		{ name = "x", type = "number" },
	},
	valuetype = "number",
}
