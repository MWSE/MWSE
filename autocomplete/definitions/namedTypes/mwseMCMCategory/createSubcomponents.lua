return {
	type = "method",
	description = [[Creates UI element tree for all the given components by calling `component:create`.]],
	arguments = {
		{ name = "parentBlock",	type = "tes3uiElement" },
		{ name = "components", type = "mwseMCMComponent.new.data[]", description = "This table is described at each Component's `new` method." }
	}
}
