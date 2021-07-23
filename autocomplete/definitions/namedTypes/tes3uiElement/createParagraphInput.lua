return {
	type = "method",
	description = [[Creates a multi-line text input element, with line wrapping on. To receive input the keyboard must be captured with ``tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to edit by setting the ``text`` property.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.lengthLimit``: Maximum input length. Default is ``1023``.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
	},
    valuetype = "tes3uiElement",
}