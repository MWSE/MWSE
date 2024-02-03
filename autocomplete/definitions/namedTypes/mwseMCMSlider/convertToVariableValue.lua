return {
	type = "method",
	description = [[This function specifies how values stored in the slider widget should correspond to values stored in the `variable` field.
		
This conversion is necessary because the widget can only store whole numbers, and the range of allowed values must start at 0, while the corresponding `variable` can store decimal numbers and the range can start at any number.
In the vast majority of use-cases, you do not need to call this method directly.]],
	arguments = {
		{ name = "widgetValue", type = "number" }
	},
	returns = {{
		name = "variableValue", type = "number"
	}}
}
