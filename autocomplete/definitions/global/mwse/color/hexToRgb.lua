return {
	type = "function",
	description = [[Converts given hex code in (AA)RRGGBB format to RGB(A).]],
	arguments = {
		{ name = "rgb", type = "string" },
	},
	returns = {{ name = "rgb", type = "mwseColorTable|mwseColorATable" }},
}
