return {
	type = "function",
	description = [[This function returns a localized name for the mouse wheel direction. You can pass [`mouseWheel`](https://mwse.github.io/MWSE/types/mwseKeyMouseCombo/#mousewheel) field of `mwseKeyMouseCombo` or [`e.delta`](https://mwse.github.io/MWSE/events/mouseWheel/#event-data) from `mouseWheel` event data.]],
	arguments = {
		{ name = "mouseWheel", type = "integer", optional = true },
	},
	returns = {{ name = "result", type = "string|nil" }}
}
