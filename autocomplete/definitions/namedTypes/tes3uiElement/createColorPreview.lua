return {
	type = "method",
	description = [[Creates a color preview widget. It's made of a rect element of provided color and an image. The image shows current RGBA color over the checkered background.

Color preview specific properties can be accessed through the `widget` property. The widget type for color previews is [`tes3uiColorPreview`](https://mwse.github.io/MWSE/types/tes3uiColorPreview/).]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "id", type = "string|integer", description = "An identifier to help find this element later.", optional = true },
			{ name = "color", type = "mwseColorTable|ffiImagePixel", optional = true, default = "{ r = 1.0, g = 1.0, b = 1.0 }", description = "The color of the preview." },
			{ name = "hasAlphaPreview", type = "boolean", optional = true, default = true, description = "If `true`, the color preview in addition to colored rect also has an image that shows current color over a checkered background." },
			{ name = "alpha", type = "number", optional = true, default = 1, description = "The alpha value of the preview." },
			{ name = "width", type = "integer", optional = true, default = 64, description = "The width of the individual preview element." },
			{ name = "height", type = "integer", optional = true, default = 64, description = "The height of the individual preview element." },
			{ name = "flowDirection", type = "tes3.flowDirection", optional = true, default = "tes3.flowDirection.leftToRight", description = "Determines if the color preview is horizontal or vertical." },
			{ name = "checkerSize", type = "integer", optional = true, default = 16, description = "The size of individual square in the color preview image in pixels." },
			{ name = "lightGray", type = "mwseColorTable", optional = true, default = "{ r = 0.7, g = 0.7, b = 0.7 }", description = "The color of lighter squares in the color preview image." },
			{ name = "darkGray", type = "mwseColorTable", optional = true, default = "{ r = 0.5, g = 0.5, b = 0.5 }", description = "The color of darker squares in the color preview image." },
		},
	}},
	returns = { name = "result", type = "tes3uiElement" },
}
