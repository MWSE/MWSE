return {
	type = "method",
	description = [[This function specifies how values stored in the `variable` field should correspond to values displayed by this setting.]],
	arguments = {
		{ name = "variableValue", type = "number" }
	},
	returns = {{
		name = "labelValue", type = "number|string"
	}},
}
