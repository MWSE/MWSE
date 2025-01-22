return {
	type = "method",
	description = "Given an `optionValue`, this method will retrieve the first [`mwseMCMDropdownOption`](./mwseMCMDropdownOption.md) \z
		with a matching `value`, if such an option exists.",
	arguments = {
		{ name = "optionValue", type = "unknown", optional = true, default = "self.variable.value" }
	},
	returns = {{
			name = "option",
			type = "mwseMCMDropdownOption|nil",
			description = "The corresponding [`mwseMCMDropdownOption`](./mwseMCMDropdownOption.md)."
	}}
}
