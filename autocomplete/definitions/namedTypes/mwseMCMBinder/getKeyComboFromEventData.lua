return {
	type = "method",
	description = [[Used to convert raw input event data into `mwseKeyMouseCombo`.]],
	arguments = {
		{ name = "e", type = "keyDownEventData|mouseWheelEventData|mouseButtonDownEventData|mwseKeyMouseCombo" }
	},
	returns = {{ name = "keyCombo", type = "mwseKeyMouseCombo" }}
}
