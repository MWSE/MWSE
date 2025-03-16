return {
	type = "method",
	description = [[This function specifies how values stored in the `variable` field should correspond to values displayed by this setting.
The default behavior is to return the `label` of the [`mwseMCMDropdownOption`](./mwseMCMDropdownOption.md) with a given `variableValue`.]],
	arguments = {{ 
		name = "variableValue", 
		type = "unknown", 
		description = "The value of a [`mwseMCMDropdownOption`](./mwseMCMDropdownOption.md) stored in `self.options`." 
	}},
	returns = {{
		name = "labelValue", 
		type = "string",
		description = "The label of the corresponding [`mwseMCMDropdownOption`](./mwseMCMDropdownOption.md)." 
	}},
}
