return {
	type = "method",
	description = [[Creates a horizontal quantity indicator bar.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.current``: Current (filled) value.
        | `number`_ (integer) ``element.widget.max``: Maximum value.
        | `boolean`_ ``element.widget.showText``: If text of the format "current/max" is shown. Default is ``true``.
        | `table`_ (float[3]) ``element.widget.fillColor``: Colour of filled area.
        | `number`_ (float) ``element.widget.fillAlpha``: Alpha transparency of filled area.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "current", type = "number", description = "The initial current value.", optional = true },
		{ name = "max", type = "number", description = "The initial maximum value.", optional = true },
	},
    valuetype = "tes3uiElement",
}