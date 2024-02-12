return {
	type = "method",
	description = [[Creates a new Component of given class or returns the given Component.]],
	arguments = {{
		name = "componentData",
		type = "mwseMCMComponent|table",
		tableParams = {
			{ name = "class", type = "string", description = [[The component type to get. On of the following:
		- `"Template"`
		- `"ExclusionsPage"`
		- `"FilterPage"`
		- `"MouseOverPage"`
		- `"Page"`
		- `"SideBarPage"`
		- `"Category"`
		- `"SideBySideBlock"`
		- `"ActiveInfo"`
		- `"Hyperlink"`
		- `"Info"`
		- `"MouseOverInfo"`
		- `"Setting"`
		- `"Button"`
		- `"OnOffButton"`
		- `"YesNoButton"`
		- `"CycleButton"`
		- `"KeyBinder"`
		- `"Dropdown"`
		- `"TextField"`
		- `"ParagraphField"`
		- `"Slider"`
		- `"DecimalSlider"`
		- `"PercentageSlider"`]] 
			},
			{ name = "label", type = "string", optional = true, description = "The label text to set for the new component. Not all component types have a label." },
			{ name = "indent", type = "integer", optional = true, default = 12, description = "The left padding size in pixels. Only used if the `childIndent` isn't set on the parent component." },
			{ name = "childIndent", type = "integer", optional = true, description = "The left padding size in pixels. Used on all the child components." },
			{ name = "paddingBottom", type = "integer", optional = true, default = 4, description = "The bottom border size in pixels. Only used if the `childSpacing` is unset on the parent component." },
			{ name = "childSpacing", type = "integer", optional = true, description = "The bottom border size in pixels. Used on all the child components." },
			{ name = "inGameOnly", type = "boolean", optional = true, default = false },
			{ name = "postCreate", type = "fun(self: mwseMCMComponent)", optional = true, description = "Can define a custom formatting function to make adjustments to any element saved in `self.elements`." },
			{ name = "parentComponent", type = "mwseMCMComponent", optional = true },
		}
	}},
	returns = {{
		name = "component", type = "mwseMCMComponent"
	}}
}
