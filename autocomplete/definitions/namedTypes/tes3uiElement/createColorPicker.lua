return {
	type = "method",
	description = [[Creates a color picker widget.

Color picker specific properties can be accessed through the `widget` property. The widget type for color pickers is [`tes3uiColorPicker`](https://mwse.github.io/MWSE/types/tes3uiColorPicker/).]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "id", type = "string|integer", description = "An identifier to help find this element later.", optional = true },
			{ name = "initialColor", type = "mwseColorTable", optional = true, default = "{ r = 1.0, g = 1.0, b = 1.0 }", description = "The initial color for the picker." },
			{ name = "alpha", type = "boolean", optional = true, default = false, description = "If `true` the picker will also allow picking an alpha value." },
			{ name = "initialAlpha", type = "number", optional = true, default = 1.0, description = "The initial alpha value." },
			{ name = "vertical", type = "boolean", optional = true, default = false, description = "If `true`, saturation, hue and alpha bars and color previews are created in the second row below the main picker. If `false` they are created in the same row as the main picker." },
			{ name = "showDataRow", type = "boolean", optional = true, default = true, description = "If `true` the picker will have a text input below the main picker for changing the current hexadecimal RGB(A) value." },
			{ name = "showSaturationSlider", type = "boolean", optional = true, default = true, description = "If `true` the picker will have a slider below the main picker that moves current selection horizontally (in the saturation axis)." },
			{ name = "showSaturationPicker", type = "boolean", optional = true, default = true, description = "If `true` the picker will have an additional bar for changing saturation of the currently selected color." },
			{ name = "height", type = "integer", optional = true, default = 256, description = "The height of the main, hue, and optionally alpha and saturation, pickers." },
			{ name = "mainWidth", type = "integer", optional = true, default = 256, description = "The width of the main picker." },
			{ name = "hueWidth", type = "integer", optional = true, default = 32, description = "The width of pickers for hue, and optionally alpha and saturation." },
			{ name = "showPreviews", type = "boolean", optional = true, default = true, description = "If `false` the picker won't have any color preview widgets." },
			{ name = "showOriginal", type = "boolean", optional = true, default = true, description = "If `true` the picker will have a preview widget that shows original color below the currently picked color. Clicking on the original color will reset current color to original color." },
			{ name = "previewHeight", type = "integer", optional = true, default = 64, description = "If color picker has color previews, this will be the height of individual preview image." },
			{ name = "previewWidth", type = "integer", optional = true, default = 64, description = "If color picker has color previews, this will be the width of individual preview image." },
		},
	}},
	returns = {{ name = "result", type = "tes3uiElement" }},
}
