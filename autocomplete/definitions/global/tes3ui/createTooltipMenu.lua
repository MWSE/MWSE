return {
	type = "function",
	description = [[Creates a tooltip menu, which can be an empty menu, an item tooltip, a skill tooltip, or a spell tooltip. This should be called from within a tooltip event callback. These automatically follow the mouse cursor, and are also destroyed automatically when the mouse leaves the originating element. Creating an object tooltip will invoke the uiObjectTooltip event. Creating a tooltip with no argument will create an empty tooltip.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "object", type = "tes3object|string", optional = true, description = "The object to create a tooltip for." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The itemData for the object, if providing an object." },
			{ name = "spell", type = "tes3spell", optional = true, description = "The spell to create a tooltip for." },
			{ name = "skill", type = "tes3skill", optional = true, description = "The skill to create a tooltip for." },
		},
	}},
	valuetype = "tes3uiElement",
	examples = {
		["itemTooltip"] = {
			title = "Add an item tooltip to a new element",
			description = "This demonstrates adding an item tooltip to a button using the help event."
		},
	}
}
