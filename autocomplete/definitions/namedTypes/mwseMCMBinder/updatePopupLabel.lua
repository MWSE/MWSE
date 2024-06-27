return {
	type = "method",
	description = [[Updates the label that shows currently bound key combination in the rebind popup to the given combination.]],
	arguments = {
		{ name = "keyCombo", type = "mwseKeyMouseCombo" }
	},
	returns = {{ name = "updated", type = "boolean" }}
}
