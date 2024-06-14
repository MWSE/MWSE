return {
	type = "function",
	description = [[This function returns a localized name of the mouse button. You can pass [`mouseButton`](https://mwse.github.io/MWSE/types/mwseKeyMouseCombo/#mousebutton) field of `mwseKeyMouseCombo` or [`e.button`](https://mwse.github.io/MWSE/events/mouseButtonDown/#event-data) from `mouseButtonDown` event data.]],
	arguments = {
		{ name = "buttonIndex", type = "integer", optional = true },
	},
	returns = {{ name = "result", type = "string|nil" }}
}
