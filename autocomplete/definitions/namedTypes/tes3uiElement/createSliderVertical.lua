return {
	type = "method",
	description = [[Creates a vertical slider.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.current``: Current value.
        | `number`_ (integer) ``element.widget.max``: Maximum value.
        | `number`_ (integer) ``element.widget.step``: Amount changed by up and down arrow buttons.
        | `number`_ (integer) ``element.widget.jump``: Amount changed by clicking inside the slider area.

    Custom events used with register:
        | ``"PartScrollBar_changed"``: Triggers on value change; moving the slider is not enough if the value is the same.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "current", type = "number", description = "The initial value for the slider." },
		{ name = "max", type = "number", description = "The maximum value for the slider." },
		{ name = "step", type = "number", description = "Amount changed by left and right arrow buttons. The default is 1.", optional = true },
		{ name = "jump", type = "number", description = "Amount changed by clicking inside the slider area. The default is 5.", optional = true },
	},
    valuetype = "tes3uiElement",
}