return {
	type = "method",
	description = [[Creates a new tab in the container. This will add a tab button to the top of the element, as well as a new block for the contents, which will be returned.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string", description = "The unique identifier for the tab." },
			{ name = "name", type = "string", optional = true, description = "The text used for the tab's selector button. If not provided, the ID will be used." },
		},
	}},
	returns = "tes3uiElement",
}