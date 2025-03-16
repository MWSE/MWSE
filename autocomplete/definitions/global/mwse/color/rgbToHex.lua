return {
	type = "function",
	description = [[Converts given RGB(A) color to an HTML hex code in (AA)RRGGBB format. Given color channels must be in [0, 1] range.]],
	arguments = {
		{ name = "rgb", type = "mwseColorTable|mwseColorATable|niColor|niColorA|ffiImagePixel" },
	},
	returns = {{ name = "hex", type = "string" }},
}
