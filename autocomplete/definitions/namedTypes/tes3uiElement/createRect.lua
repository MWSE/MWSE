return {
	type = "method",
	description = [[Creates a filled rectangle. The rectangle is displayed as filled with the element's colour. It supports alpha compositing.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "color", type = "table", description = "The fill colour for the element.", optional = true },
	},
    valuetype = "tes3uiElement",
}