return {
	type = "method",
	description = [[Creates a collection of elements meant to represent a tab-based interface. With the widget of this element, tabs can be added with `:addTab`, which will contain the content element of that tab. When the user clicks on each tab, other tab content panes will be hidden, in favor of the one associated with the clicked tab. See [`tes3uiTabContainer`](https://mwse.github.io/MWSE/types/tes3uiTabContainer/) for a what the widget provides.]],
	arguments = { {
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string|number", description = "An identifier to help find this element later.", optional = true },
			{ name = "showArrows", type = "boolean", default = false, description = "If set, displays left and right arrows on either side of the tab list. This can help when navigating many tabs." },
		},
	} },
	valuetype = "tes3uiElement",
}
