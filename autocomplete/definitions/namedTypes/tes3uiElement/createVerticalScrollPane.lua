return {
	type = "method",
	description = [[Creates a vertically scrolling pane. Useful as a list box.

    Custom widget properties:
        | `number`_ ``element.widget.positionY``: Vertical scroll offset in pixels.
        | `boolean`_ ``element.widget.scrollbarVisible``: Set if the scrollbar is displayed.

    Custom widget methods:
        | ``element.widget:contentsChanged()``: Call to update scroll bar slider and limits after adding or removing elements to the content container. Because content size depends on layout, this must be run after a menu:updateLayout(). Only required if the content size changes.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
	},
    valuetype = "tes3uiElement",
}