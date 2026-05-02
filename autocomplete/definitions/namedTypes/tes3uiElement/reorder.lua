return {
	type = "method",
	description = [[Re-orders an element to before or after a sibling element. Provide either a `before` or `after` parameter.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "before", type = "tes3uiElement", optional = true, description = "The calling element will be moved to before this element." },
			{ name = "after", type = "tes3uiElement", optional = true, description = "The calling element will be moved to after this element." },
		},
	}},
	valuetype = "boolean",
}