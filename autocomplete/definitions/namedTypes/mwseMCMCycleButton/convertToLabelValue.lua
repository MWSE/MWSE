return {
	type = "method",
	description = [[This function specifies how values stored in the `variable` field should correspond to values displayed by this CycleButton.
The default behavior is to return the `text` of the [`mwseMCMCycleButton`](./mwseMCMCycleButton.md) with a given `variableValue`.]],
	arguments = {{
		name = "variableValue",
		type = "any",
		description = "The value of a [`mwseMCMCycleButton`](./mwseMCMCycleButton.md) stored in `self.options`."
	}},
	returns = {{
		name = "labelValue",
		type = "string",
		description = "The label of the corresponding [`mwseMCMCycleButton`](./mwseMCMCycleButton.md)."
	}},
}
