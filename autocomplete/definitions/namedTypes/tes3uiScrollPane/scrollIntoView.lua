return {
	type = "method",
	description = [[Scrolls the pane vertically to ensure that the given descendant element is fully visible. If the element is already fully visible, the scroll position is left unchanged.]],
	arguments = { {
		name = "child",
		type = "tes3uiElement",
		description = "A descendant element of the scroll pane's content element. Nested descendants are supported.",
	} },
	valuetype = "boolean",
}
