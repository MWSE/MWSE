return {
	type = "function",
	description = [[This function returns a localized name for a key combination.]],
	arguments = {
		{ name = "keyCombo", type = "mwseKeyCombo|mwseKeyMouseCombo", optional = true },
	},
	returns = {{ name = "result", type = "string|nil" }}
}
